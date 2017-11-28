/* LUCAS SULZBACH GRR20171595 */

#include "graphics.h"

/* Inicializa o SDL2 */

void initGraphics()
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Wireframe", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    if (!window)
    {
        fprintf(stderr, "Graphics Error: Could not create window\n");
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        fprintf(stderr, "Graphics Error: Could not create renderer\n");
        exit(EXIT_FAILURE);
    }

    EnMouse = 0;
}

/*********************/

/* Plota o objeto, desenhando as arestas */

char plotObj(edge_t *Edges, unsigned int EdgeNum, threeD_t *Cam, twoD_t *dir, float *zoom)
{ 
    SDL_Event e;
    signed char status = -1;
    while (status == -1) 
    { 
        if (SDL_PollEvent(&e)) 
        { 
            if (e.type == SDL_QUIT)
            {
                dir->x = 0;
                dir->y = 0; 
                status = 0;
#ifdef __DEBUG__
                printf("Event: Quit\n");
#endif
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_LEFT)
                {
                    dir->x = -1;
                    dir->y = 0;
                    status = 1;
#ifdef __DEBUG__
                    printf("Event: Key Left\n");
#endif
                }
                else if (e.key.keysym.sym == SDLK_RIGHT)
                {
                    dir->x = 1;
                    dir->y = 0;
                    status = 1;
#ifdef __DEBUG__
                    printf("Event: Key Right\n");
#endif
                }
                else if (e.key.keysym.sym == SDLK_UP)
                {
                    dir->x = 0;
                    dir->y = 1;
                    status = 1;
#ifdef __DEBUG__
                    printf("Event: Key Up\n");
#endif
                }
                else if (e.key.keysym.sym == SDLK_DOWN)
                {
                    dir->x = 0;
                    dir->y = -1;
                    status = 1;
#ifdef __DEBUG__
                    printf("Event: Key Down\n");
#endif
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                EnMouse = 1;
#ifdef __DEBUG__
                printf("Event: Mouse Press\n");
#endif
                Start.x = e.motion.x;
                Start.y = e.motion.y;
            }
            else if (e.type == SDL_MOUSEMOTION && EnMouse)
            {
#ifdef __DEBUG__
                printf("Event: Mouse Motion\n");
#endif
                if (e.motion.x - Start.x > 0)
                    dir->x = 1;
                else if (e.motion.x - Start.x < 0)
                    dir->x = -1;
                else
                    dir->x = 0;

                if (e.motion.y - Start.y > 0)
                    dir->y = 1;
                else if (e.motion.y - Start.y < 0)
                    dir->y = -1;
                else
                    dir->y = 0;

                if (dir->x || dir->y)
                {
                    Start.x = e.motion.x;
                    Start.y = e.motion.y;
                    status = 1;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONUP)
            {
                EnMouse = 0;
            }
            else if (e.type == SDL_MOUSEWHEEL)
            {
#ifdef __DEBUG__
                printf("Event: Mouse Scroll\n");
#endif
                if (e.wheel.y == 1)
                {
                    *zoom += 0.01;
                    dir->x = 0;
                    dir->y = 0;
                    status = 1;
                }
                else if (e.wheel.y == -1)
                {
                    *zoom -= 0.01;
                    dir->x = 0;
                    dir->y = 0;
                    status = 1;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderClear(renderer);

        for (unsigned int i = 0; i < EdgeNum; i++)
            lineRGBA(renderer, Edges[i].Start->x, Edges[i].Start->y, Edges[i].End->x, Edges[i].End->y, 255, 255, 255, 100);

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