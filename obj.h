#ifndef __OBJ__
#define __OBJ__

#include "list.h"
#include <string.h>
#include <math.h>

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

    unsigned int VertNum;
    list_t *VertInfo;
    threeD_t *RawVerts;
    twoD_t *ProjVerts;

    list_t *FaceInfo;

    unsigned int EdgeNum;
    list_t *EdgeInfo;
    edge_t *Edges;

} obj_t;

/**********/

#define MAXFLOATSIZE 30
#define MAXINTSIZE 30

void initObj(obj_t *Obj);
threeD_t *getRawVerts(list_t *VertInfo, unsigned int *VertNum);
void initCam(threeD_t *Cam, threeD_t *RawVerts, unsigned int VertNum);
void moveCam(threeD_t *Cam, char dir);
void getProjVerts(threeD_t *RawVerts, twoD_t *ProjVerts, unsigned int VertNum, threeD_t Cam);
void convertToScrCoords(twoD_t *ProjVerts, unsigned int VertNum, unsigned int W, unsigned int H);
edge_t *getEdges(list_t *EdgeInfo, unsigned int *EdgeNum, list_t *FaceInfo);

#endif