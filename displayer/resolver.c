// Tool for resolving the generated maze with A* (https://en.wikipedia.org/wiki/A*_search_algorithm):
// \
gcc resolver.c list.c bin_heap.c -lSDL2 -o resolve && \
g++ ../linux_maze.cpp ../src/*.cpp -I../src -o maze && \
./maze out | ./resolve && echo ok

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "bin_heap.h"
#include "list.h"

#define MAP_SIZE 100

typedef enum{
    nothing=0,
    N=1U, S=2U, E=4U, W=8U,
} t_cell;
#define is_open_(c,d) ((c)&(d))

#define FINAL_X 5
#define FINAL_Y 12

#if 0
// Heuristic Square Euclidean function
int heuristic(int x, int y) {
	return (x-FINAL_X)*(x-FINAL_X)+(y-FINAL_Y)*(y-FINAL_Y);
}
#else
// Heuristic Manhattan function 
int heuristic(int x, int y) {
    return abs(x-FINAL_X)+abs(y-FINAL_Y);
}
#endif

int main(int argc, char ** argv)
{
    SDL_Event event;
 
    SDL_Init(SDL_INIT_VIDEO);
 
    SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture * texture[16];
    for(size_t i=0; i<=15; ++i)
    {
        char fn[10];
        sprintf(fn, "%d.bmp", i);
        SDL_Surface *image = SDL_LoadBMP(fn);
        texture[i] = SDL_CreateTextureFromSurface(renderer, image);
        SDL_FreeSurface(image);
    }

    int maze[MAP_SIZE][MAP_SIZE];

    for(size_t i=0; i<MAP_SIZE; ++i)
    {
        for(size_t j=0; j<MAP_SIZE; ++j)
        {
            int n;
            scanf("%2d ", &n);
            maze[i][j] = n;
        }
        scanf("\n");
    }

    size_t open_size = 0;
    NODE_TYPE* open[MAX_SIZE];
    LIST_TYPE* close=NULL;

    // we basically suppose that we will always have memory for malloced data
    enqueue(open, create_node(50,50,0,heuristic(50,50)), &open_size);
    NODE_TYPE* current = dequeue(open, &open_size);
    LIST_TYPE* element = create(current->x, current->y, current->gscore);
    push_front(&close, element);
    SDL_Rect dstrect = { 0, 0, 8, 8};

    // note: open_size+1 as we just dequeue one node
    while(!(open_size+1 == 0 || (current->x == FINAL_X && current->y == FINAL_Y)))
    {
        dstrect.y = current->x*8; // yes... terrible names...
        dstrect.x = current->y*8;
        SDL_RenderCopy(renderer, texture[maze[current->x][current->y]], NULL, &dstrect);
        SDL_RenderPresent(renderer);

        if(is_open_(maze[current->x][current->y], N) && current->x>0)
        {
            LIST_TYPE* element=create(current->x-1, current->y, current->gscore+1);
            if(!update_if_exists(close, element))
            {
                enqueue(open, \
                    create_node(current->x-1,current->y, \
                        current->gscore+1, \
                        current->gscore+heuristic(current->x-1,current->y)), \
                    &open_size);
                push_front(&close, element);
            }
        }
        if(is_open_(maze[current->x][current->y], S) && current->x<MAP_SIZE)
        {
            LIST_TYPE* element=create(current->x+1, current->y, current->gscore+1);
            if(!update_if_exists(close, element))
            {
                enqueue(open, \
                    create_node(current->x+1,current->y, \
                        current->gscore+1, \
                        current->gscore+heuristic(current->x+1,current->y)), \
                    &open_size);
                push_front(&close, element);
            }
        }
        if(is_open_(maze[current->x][current->y], E) && current->y<MAP_SIZE)
        {
            LIST_TYPE* element=create(current->x, current->y+1, current->gscore+1);
            if(!update_if_exists(close, element))
            {
                enqueue(open, \
                    create_node(current->x,current->y+1, \
                        current->gscore+1, \
                        current->gscore+heuristic(current->x,current->y+1)), \
                    &open_size);
                push_front(&close, element);
            }
        }
        if(is_open_(maze[current->x][current->y], W) && current->y>0)
        {
            LIST_TYPE* element=create(current->x, current->y-1, current->gscore+1);
            if(!update_if_exists(close, element))
            {
                enqueue(open, \
                    create_node(current->x,current->y-1, \
                        current->gscore+1, \
                        current->gscore+heuristic(current->x,current->y-1)), \
                    &open_size);
                push_front(&close, element);
            }
        }

        current = dequeue(open, &open_size);
    }

    if ((current->x == FINAL_X && current->y == FINAL_Y))
    {
        const Uint32 format = SDL_PIXELFORMAT_ARGB8888;
        SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, 800, 800, 32, format);
        SDL_RenderReadPixels(renderer, NULL, format, surface->pixels, surface->pitch);
        SDL_SaveBMP(surface, "screenshot.bmp");
        SDL_FreeSurface(surface);
        printf("path found in %d moves\n", current->gscore);
    } else {
        printf("*** NO PATH FOUND !\n");
    }

    while (SDL_WaitEvent(&event) && event.type != SDL_QUIT);
    for(size_t i=0; i<=15; ++i)
    {
        SDL_DestroyTexture(texture[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return !(current->x == FINAL_X && current->y == FINAL_Y); // returns 0 if path found
}