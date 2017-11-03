#include "graphics.h"

void plotObj(twoD_t *Verts, unsigned int VertsNum, edge_t *Edges, unsigned int EdgeNum, unsigned int WIDTH, unsigned int HEIGHT)
{ 
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window *window = SDL_CreateWindow("Wireframe", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_OPENGL); 
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); 
    
    SDL_Event e; 
    
    int quit = 0; 
    while (!quit) 
    { 
        if (SDL_PollEvent(&e)) 
        { 
            if (e.type == SDL_QUIT) 
                quit = 1; 
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderClear(renderer);

        for (unsigned int i = 0; i < EdgeNum; i++)
            lineRGBA(renderer, Verts[Edges[i].Start].x, Verts[Edges[i].Start].y, Verts[Edges[i].End].x, Verts[Edges[i].End].y, 255, 255, 255, 100);

        SDL_RenderPresent(renderer); 
    } 
    
    SDL_DestroyRenderer(renderer); 
    SDL_DestroyWindow(window); 
    SDL_Quit(); 
}