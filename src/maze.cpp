#include "maze.hpp"
#include "Compass.hpp"
#include "Exit.hpp"

#define display_walls(c) (is_open_((c),N)?' ':'N')<<(is_open_((c),S)?' ':'S')<<\
                         (is_open_((c),E)?' ':'E')<<(is_open_((c),W)?' ':'W')

#define display_paths(c) (is_open_((c),N)?'N':' ')<<(is_open_((c),S)?'S':' ')<<\
                         (is_open_((c),E)?'E':' ')<<(is_open_((c),W)?'W':' ')

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

#define absolute_x_to_map(x) (x)-x0
#define absolute_y_to_map(y) (y)-y0

bool Maze::is_in_map(int32_t x, int32_t y)
{
    return x>=x0 && y>=y0 && x<x0+MAP_SIZE && y<y0+MAP_SIZE;
}

bool Maze::is_set(int32_t x, int32_t y, t_cell d)
{
    return maze[x-x0][y-y0]&d;
}

void Maze::set_level(int32_t x, int32_t y)
{
    items.clear();
    
    int32_t xi, yi;

    // set the compas in the [-2,+2] square
    xi=rand()%4-2;
    if (xi<0) xi=x+xi-0;
    else xi=x+xi+0;
    yi=rand()%4-2;
    if (yi<0) yi=y+yi-0;
    else yi=y+yi+0;

    items.push_back(new Compass(xi,yi));

    // set the exit in the [-30,+30] square but not in the [-10,+10]
    xi=rand()%40-20;
    if (xi<0) xi=x+xi-10;
    else xi=x+xi+10;
    yi=rand()%40-20;
    if (yi<0) yi=y+yi-10;
    else yi=y+yi+10;

    items.push_back(new Exit(xi,yi));
}

void Maze::load_map(int32_t x, int32_t y)
{
    x0 = x-MAP_SIZE/2;
    y0 = y-MAP_SIZE/2;

    // set all walls closed by default
    for (auto i=0; i<MAP_SIZE; ++i)
    {
        for (auto j=0; j<MAP_SIZE; ++j)
        {
            maze[i][j] = 0;
        }
    }

    // sidewinder algorithm + multiple south path as in Eller's algorithm

    // construct maze, the [0][0] cell is [x-MAP_SIZE/2][y-MAP_SIZE/2]
    // set rng to the row above the first in maze[][]
    int32_t i=x0-1;
    set_rng(i);
    while (i<x0+MAP_SIZE)
    {
        int32_t j=0;
        while (j<y0+MAP_SIZE)
        {
            // create a EW corridor
            int32_t s=0;
            while(coinbias(0.67))
            {
                if(is_in_map(i,j+s))
                {
                    set_open_(maze[absolute_x_to_map(i)][absolute_y_to_map(j+s)],E);
                    if (j+s<y0+MAP_SIZE-1)
                    {
                        set_open_(maze[absolute_x_to_map(i)][absolute_y_to_map(j+s+1)],W);
                    }
                } else if(j+s==y0-1 && is_in_map(i,j+s+1))
                {
                    set_open_(maze[absolute_x_to_map(i)][0],N); 
                }
                ++s;
            }
            // ensure we have at least one path to south
            int32_t to_south = (s==0)?0:rand()%s;
            if(is_in_map(i,j+to_south))
            {
                set_open_(maze[absolute_x_to_map(i)][absolute_y_to_map(j+to_south)],S);
                if (i<x0+MAP_SIZE-1)
                {
                    set_open_(maze[absolute_x_to_map(i+1)][absolute_y_to_map(j+to_south)],N);
                }
            } else if(i==x0-1 && is_in_map(i+1,j+to_south))
            {
               set_open_(maze[0][absolute_y_to_map(j+to_south)],N); 
            }
            // create new random path to south
            while(s>=0)
            {
                if(coinbias(0.33) && is_in_map(i,j))
                {
                    set_open_(maze[absolute_x_to_map(i)][absolute_y_to_map(j)],S);
                    if (i<x0+MAP_SIZE-1)
                    {
                        set_open_(maze[absolute_x_to_map(i+1)][absolute_y_to_map(j)],N);
                    }
                }
                --s;
                ++j;
            }
        }
        next_rng();
        ++i;
    }
}

void Maze::generate(int32_t x, int32_t y)
{
    set_level(x,y);
    load_map(x,y);
}

std::string Maze::display_cell(int32_t x, int32_t y, uint32_t status)
{
    std::string s = SSTR("Pos: "<<x<<","<<y);
//    s+= SSTR(" Walls: "<<display_walls(maze[x-x0][y-y0]));
    s+= SSTR(" Possible paths: "<<display_paths(maze[x-x0][y-y0]));
    for(auto &i:items)
        s+=SSTR(i->display_cell(x,y,status));
    return s;
}
