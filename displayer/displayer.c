// Tool for checking the generated maze :
// \
gcc displayer.c -lSDL2 -o display && \
g++ ../linux_maze.cpp ../src/*.cpp -I../src -o maze && \
./maze out > lab.txt && \
diff lab.txt <(cat lab.txt | ./display) && echo ok

#include <SDL2/SDL.h>
#include <stdio.h>
 
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

    SDL_Rect dstrect = { 0, 0, 8, 8};
    for(size_t i=0; i<100; ++i)
    {
        dstrect.y = i*8;
        for(size_t j=0; j<100; ++j)
        {
            int n;
            dstrect.x = j*8;
            scanf("%2d ", &n);
            printf("%2d ", n);
            SDL_RenderCopy(renderer, texture[n], NULL, &dstrect);
        }
        scanf("\n");
        printf("\n");
    }
    SDL_RenderPresent(renderer);
    
    const Uint32 format = SDL_PxELFORMAT_ARGB8888;
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, 800, 800, 32, format);
    SDL_RenderReadPxels(renderer, NULL, format, surface->pxels, surface->pitch);
    SDL_SaveBMP(surface, "screenshot.bmp");
    SDL_FreeSurface(surface);

    while (SDL_WaitEvent(&event) && event.type != SDL_QUIT);

    for(size_t i=0; i<=15; ++i)
    {
        SDL_DestroyTexture(texture[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
 
    return 0;
}