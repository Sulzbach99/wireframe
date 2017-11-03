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

/* Aresta */

typedef struct {

    twoD_t *Start;
    twoD_t *End;

} edge_t;

/**********/

/* Objeto */

typedef struct {

    list_t *VertInfo;
    threeD_t *RawVerts;
    twoD_t *ProjVerts;
    float Xmin, Ymin;
    float Xmax, Ymax;
    float Xcen, Ycen;
    float Xdif, Ydif;
    float Scale;
    list_t *FaceInfo;
    edge_t *Edges;

} obj_t;

/**********/

void *Malloc(int size);

#endif