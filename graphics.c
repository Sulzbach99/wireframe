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

char plotObj(twoD_t *Verts, edge_t *Edges, unsigned int EdgeNum, threeD_t *Cam)
{ 
    SDL_Event e; 
    
    signed char status = -1; 
    while (status == -1) 
    { 
        if (SDL_PollEvent(&e)) 
        { 
            if (e.type == SDL_QUIT) 
                status = 0;

            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_LEFT)
                    status = 1;
                else if (e.key.keysym.sym == SDLK_RIGHT)
                    status = 2;
                else if (e.key.keysym.sym == SDLK_UP)
                    status = 3;
                else if (e.key.keysym.sym == SDLK_DOWN)
                    status = 4;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderClear(renderer);

        for (unsigned int i = 0; i < EdgeNum; i++)
            lineRGBA(renderer, Verts[Edges[i].Start].x, Verts[Edges[i].Start].y, Verts[Edges[i].End].x, Verts[Edges[i].End].y, 255, 255, 255, 100);

        SDL_RenderPresent(renderer);
    } 

    return status;
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