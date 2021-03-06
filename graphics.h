/* LUCAS SULZBACH GRR20171595 */

#ifndef __GRAPHICS__
#define __GRAPHICS__

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "obj.h"

#define WIDTH 800
#define HEIGHT 600

SDL_Window *window;
SDL_Renderer *renderer;

char EnMouse;
twoD_t Start; 

void initGraphics();
char plotObj(edge_t *Edges, unsigned int EdgeNum, threeD_t *Cam, twoD_t *dir, float *zoom);
void killGraphics();

#endif