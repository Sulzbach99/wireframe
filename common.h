#ifndef __COMMON__
#define __COMMON__

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

#include <stdio.h>
#include <stdlib.h>

void *Malloc(int size);

#endif