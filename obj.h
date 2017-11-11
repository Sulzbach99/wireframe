#ifndef __OBJ__
#define __OBJ__

#include "queue.h"
#include "debug.h"
#include <string.h>
#include <math.h>

/* Coordenada bidimensional */

typedef struct {

    double x;
    double y;

} twoD_t;

/****************************/

/* Coordenada tridimensional */

typedef struct {

    double x;
    double y;
    double z;

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
    double AngX, AngY;

} cam_t;

/**********/

#define NORM(u) sqrt(pow(u.x, 2) + pow(u.y, 2) + pow(u.z, 2))
#define ROTA(a, b, ang) a * cos(ang) - b * sin(ang)
#define ROTB(a, b, ang) a * sin(ang) + b * cos(ang)

#define MAXFLOATSIZE 30
#define MAXINTSIZE 30

void initObj(obj_t *Obj);
void initCam(cam_t *Cam, threeD_t *RawVerts, unsigned int VertNum);
void moveCam(cam_t *Cam, twoD_t dir);
void allocProjVerts(twoD_t **ProjVerts, unsigned int VertNum);
void getRawVerts(threeD_t **RawVerts, queue_t *VertInfo, unsigned int *VertNum);
void getProjVerts(threeD_t *RawVerts, twoD_t *ProjVerts, unsigned int VertNum, cam_t Cam);
void getEdges(edge_t **Edges, queue_t *EdgeInfo, unsigned int *EdgeNum, queue_t *FaceInfo);
void convertToScrCoords(twoD_t *ProjVerts, unsigned int VertNum, unsigned int W, unsigned int H);

#endif