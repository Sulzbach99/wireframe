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
    char status = 1, EnMouse = 0, EnMotion = 0;
    twoD_t Start; 
    while (status) 
    { 
        if (SDL_WaitEvent(&e)) 
        { 
            if (e.type == SDL_QUIT)
            {
                dir->x = 0;
                dir->y = 0; 
                status = 0;
#ifdef __DEBUG__
                printf("Quit\n");
#endif
            }

            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_LEFT)
                {
                    dir->x = -1;
                    dir->y = 0;
                    status = 0;
#ifdef __DEBUG__
                    printf("Key Left\n");
#endif
                }
                else if (e.key.keysym.sym == SDLK_RIGHT)
                {
                    dir->x = 1;
                    dir->y = 0;
                    status = 0;
#ifdef __DEBUG__
                    printf("Key Right\n");
#endif
                }
                else if (e.key.keysym.sym == SDLK_UP)
                {
                    dir->x = 0;
                    dir->y = 1;
                    status = 0;
#ifdef __DEBUG__
                    printf("Key Up\n");
#endif
                }
                else if (e.key.keysym.sym == SDLK_DOWN)
                {
                    dir->x = 0;
                    dir->y = -1;
                    status = 0;
#ifdef __DEBUG__
                    printf("Key Down\n");
#endif
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                EnMouse = 1;

#ifdef __DEBUG__
                printf("Mouse Button\n");
#endif
            }

            if (e.type == SDL_MOUSEMOTION && EnMouse && !EnMotion)
            {
                Start.x = e.motion.x;
                Start.y = e.motion.y;
                EnMotion = 1;
            }

            if (e.type == SDL_MOUSEMOTION && EnMotion)
            {
                if (e.motion.x - Start.x > 0)
                    dir->x = -1;
                else if (e.motion.x - Start.x < 0)
                    dir->x = 1;
                else
                    dir->x = 0;

                if (e.motion.y - Start.y > 0)
                    dir->y = -1;
                else if (e.motion.y - Start.y < 0)
                    dir->y = 1;
                else
                    dir->y = 0;

                if (dir->x || dir->y)
                    status = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderClear(renderer);

        for (unsigned int i = 0; i < EdgeNum; i++)
            lineRGBA(renderer, Verts[Edges[i].Start].x, Verts[Edges[i].Start].y, Verts[Edges[i].End].x, Verts[Edges[i].End].y, 255, 255, 255, 100);

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