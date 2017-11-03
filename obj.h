#ifndef __OBJ__
#define __OBJ__

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

/* Aresta */

typedef struct {

    unsigned int Start;
    unsigned int End;

} edge_t;

/**********/

/* Objeto */

typedef struct {

    list_t VertInfo;
    threeD_t *RawVerts;
    twoD_t *ProjVerts;
    list_t FaceInfo;
    list_t EdgeInfo;
    edge_t *Edges;

} obj_t;

/**********/

#define MAXFLOATSIZE 30

void initObj(obj_t *Obj);
void getRawVerts(obj_t *Obj);
void getProjVerts(obj_t Obj, threeD_t Cam);
void convertToScrCoords(obj_t Obj, unsigned int W, unsigned int H);

#endif