#ifndef __COMMON__
#define __COMMON__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

/* Coordenada bidimensional */

typedef struct {

    float x;
    float y;

} twoD_t;

/****************************/

/* Coordenada tridimensional */

typedef struct {

    float x;
    float y;
    float z;

} threeD_t;

/*****************************/

/* Constantes para facilitar a sintaxe */

#define GETX(coord) coord->x
#define GETY(coord) coord->y
#define GETZ(coord) coord->z

/***************************************/

void *Malloc(int size);

#endif