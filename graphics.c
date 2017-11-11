#include "graphics.h"

/* Inicializa o SDL2 */

void initGraphics()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Wireframe", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_OPENGL); 
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); 
}

/*********************/

/* Plota o objeto, desenhando as arestas */

void plotObj(twoD_t *Verts, edge_t *Edges, unsigned int EdgeNum, threeD_t *Cam, twoD_t *dir)
{ 
    SDL_Event e;
    signed char status = 1; 
    while (status) 
    { 
        if (SDL_WaitEvent(&e)) 
        { 
            if (e.type == SDL_QUIT)
            {
                dir->x = 0;
                dir->y = 0; 
                status = 0;
                printf("Quit\n");
            }

            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_LEFT)
                {
                    dir->x = -1;
                    dir->y = 0;
                    status = 0;
                    printf("Key Left\n");
                }
                else if (e.key.keysym.sym == SDLK_RIGHT)
                {
                    dir->x = 1;
                    dir->y = 0;
                    status = 0;
                    printf("Key Right\n");
                }
                else if (e.key.keysym.sym == SDLK_UP)
                {
                    dir->x = 0;
                    dir->y = 1;
                    status = 0;
                    printf("Key Up\n");
                }
                else if (e.key.keysym.sym == SDLK_DOWN)
                {
                    dir->x = 0;
                    dir->y = -1;
                    status = 0;
                    printf("Key Down\n");
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderClear(renderer);

        for (unsigned int i = 0; i < EdgeNum; i++)
            lineRGBA(renderer, Verts[Edges[i].Start].x, Verts[Edges[i].Start].y, Verts[Edges[i].End].x, Verts[Edges[i].End].y, 255, 255, 255, 50);

        SDL_RenderPresent(renderer);
    } 
}

/*****************************************/

/* Finaliza o SDL2 */

void killGraphics()
{
    SDL_DestroyRenderer(renderer); 
    SDL_DestroyWindow(window); 
    SDL_Quit();
}

/*******************/