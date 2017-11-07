#ifndef __GRAPHICS__
#define __GRAPHICS__

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "obj.h"

char plotObj(twoD_t *Verts, unsigned int VertsNum, edge_t *Edges, unsigned int EdgeNum, unsigned int WIDTH, unsigned int HEIGHT);

#endif