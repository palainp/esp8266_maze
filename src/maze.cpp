#include "maze.hpp"
#include "Compass.hpp"
#include "Exit.hpp"
#include "Helper.hpp"
#include "Teleport.hpp"
#include "Ghost.hpp"

#define display_walls(c) (is_open_((c),N)?' ':'N')<<(is_open_((c),S)?' ':'S')<<\
                         (is_open_((c),E)?' ':'E')<<(is_open_((c),W)?' ':'W')

#define display_paths(c) (is_open_((c),N)?'N':' ')<<(is_open_((c),S)?'S':' ')<<\
                         (is_open_((c),E)?'E':' ')<<(is_open_((c),W)?'W':' ')

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

#define absolute_x_to_map(x) (x)-x0
#define absolute_y_to_map(y) (y)-y0

bool Maze::is_in_map(Player &p)
{
    return is_loaded(p.x, p.y);
}

bool Maze::is_set(uint8_t cell, t_direction d)
{
    return cell&d;
}

uint8_t Maze::cell(int32_t x, int32_t y)
{
    if(is_loaded(x,y))
        return maze[x-x0][y-y0];
    else return 0;
}

bool Maze::is_loaded(int32_t x,int32_t y) {
    return x>=x0 && y>=y0 && x<x0+MAP_SIZE && y<y0+MAP_SIZE;
}

// returns a value in [-A,+A] but not in the [-B,+B]
int32_t random_in_but_not_in(int32_t A, int32_t B) {
    int32_t d=A-B;
    int32_t r = rand()%(2*d)-d;
    if(r<0) return r-B;
    else return r+B;
}

void Maze::set_level(Player &p)
{
    items.clear();
    p.clear();

    // set the compas in the [-20,+20] square but not in the [-10,+10]
    Compass *c = new Compass(p.x+random_in_but_not_in(20,10),p.y+random_in_but_not_in(20,10));
    items.push_back(c);

    // set the exit in the [-60,+60] square but not in the [-30,+30] : this may produce a map reload with the default map size[+50,-50]
    items.push_back(new Exit(p.x+random_in_but_not_in(60,30),p.y+random_in_but_not_in(60,30)));

    // set a liar helper and a another that tells the truth to help in the [-10,+10] square but not in the [-5,+5]
    items.push_back(new Helper(p.x+random_in_but_not_in(10,5),p.y+random_in_but_not_in(10,5), true, c));
    items.push_back(new Helper(p.x+random_in_but_not_in(10,5),p.y+random_in_but_not_in(10,5), false, c));

    for (auto i=0; i<10; ++i)
    {
        items.push_back(new Teleport(p.x+random_in_but_not_in(20,4),p.y+random_in_but_not_in(20,4)));
    }
    for (auto i=0; i<6; ++i)
    {
        items.push_back(new Ghost(p.x+random_in_but_not_in(14,7),p.y+random_in_but_not_in(14,7)));
    }
}

void Maze::load_map(Player &p)
{
    x0 = p.x-MAP_SIZE/2;
    y0 = p.y-MAP_SIZE/2;

    // set all walls closed by default
    for (auto i=0; i<MAP_SIZE; ++i)
    {
        for (auto j=0; j<MAP_SIZE; ++j)
        {
            maze[i][j] = 0;
        }
    }

    // sidewinder algorithm + multiple south path as in Eller's algorithm
    // WARNING: to keep consistent maze even after a map reloading, we must
    // be sure to have always the same number of rand() calls.
    // any change here can modify the whole maze...
    // TODO: what can happen if a player go into the negative part of the maze ??

    // construct maze, the [0][0] map cell is [x-MAP_SIZE/2=x0][y-MAP_SIZE/2=y0] in the giant maze
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
                if(is_loaded(i,j+s))
                {
                    set_open_(maze[absolute_x_to_map(i)][absolute_y_to_map(j+s)],E);
                    if (j+s<y0+MAP_SIZE-1)
                    {
                        set_open_(maze[absolute_x_to_map(i)][absolute_y_to_map(j+s+1)],W);
                    }
                } else if(j+s==y0-1 && is_loaded(i,j+s+1)) // if map is connected from the west
                {
                    set_open_(maze[absolute_x_to_map(i)][0],W);
                }
                ++s;
            }
            // ensure we have at least one path to south
            int32_t to_south = (s==0)?0:rand()%s;
            if(is_loaded(i,j+to_south))
            {
                set_open_(maze[absolute_x_to_map(i)][absolute_y_to_map(j+to_south)],S);
                if (i<x0+MAP_SIZE-1)
                {
                    set_open_(maze[absolute_x_to_map(i+1)][absolute_y_to_map(j+to_south)],N);
                }
            } else if(i==x0-1 && is_loaded(i+1,j+to_south)) // if map is connected from the north
            {
               set_open_(maze[0][absolute_y_to_map(j+to_south)],N);
            }
            // create new random path to south
            while(s>=0)
            {
                if(coinbias(0.05) && is_loaded(i,j))
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

void Maze::generate(Player &p)
{
    set_level(p);
    load_map(p);
}

std::string Maze::display_cell(Player &p)
{
    std::string s = SSTR("Pos: "<<p.x<<","<<p.y);
//    s+= SSTR(" Walls: "<<display_walls(maze[x-x0][y-y0]));
    s+= SSTR(" Possible paths: "<<display_paths(maze[p.x-x0][p.y-y0]));
    for(auto &i:items)
        s+=SSTR(i->display_cell(p));
    return s;
}
