/* LUCAS SULZBACH GRR20171595 */

#ifndef __OBJ__
#define __OBJ__

#include "queue.h"
#include "debug.h"
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

    twoD_t *Start;
    twoD_t *End;

} edge_t;

/**********/

/* Objeto */

typedef struct {

    unsigned int VertNum, EdgeNum;
    queue_t *VertInfo, *FaceInfo, *EdgeInfo;
    threeD_t *RawVerts;
    twoD_t *ProjVerts;
    edge_t *Edges;

} obj_t;

/**********/

/* Câmera */

typedef struct {

    threeD_t Coords;
    short AngX, AngY;

} cam_t;

/**********/

/* Trigonometria */

float Sin[360];
float Cos[360];

/*****************/

#define NORM(u) sqrt(pow(u.x, 2) + pow(u.y, 2) + pow(u.z, 2))
#define ROTA(a, b, ang) a * Cos[ang] - b * Sin[ang]
#define ROTB(a, b, ang) a * Sin[ang] + b * Cos[ang]

#define MAXFLOATSIZE 30
#define MAXINTSIZE 30

void initTrig();
void rotate(float *x, float *y, float a, float b, short ang);
void initObj(obj_t *Obj);
void initCam(cam_t *Cam, threeD_t *RawVerts, unsigned int VertNum);
void moveCam(cam_t *Cam, twoD_t dir);
void allocProjVerts(twoD_t **ProjVerts, unsigned int VertNum);
void getRawVerts(threeD_t **RawVerts, queue_t *VertInfo, unsigned int *VertNum);
void getProjVerts(threeD_t *RawVerts, twoD_t *ProjVerts, unsigned int VertNum, cam_t Cam);
void getEdges(edge_t **EdgePtr, queue_t *EdgeInfo, unsigned int *EdgeNum, queue_t *FaceInfo, twoD_t *ProjVerts);
void convertToScrCoords(twoD_t *ProjVerts, unsigned int VertNum, unsigned int W, unsigned int H, float zoom);

#endif