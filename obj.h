#ifndef __OBJ__
#define __OBJ__

#include "list.h"
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

/* Câmera */

typedef struct {

    threeD_t Coords;
    double Radius;

} cam_t;

/**********/

#define NORM(u) sqrt(pow(u.x, 2) + pow(u.y, 2) + pow(u.z, 2))
#define ORTX(u, v) u.y * v.z - v.y * u.z
#define ORTY(u, v) u.z * v.x - v.z * u.x
#define ORTZ(u, v) u.x * v.y - v.x * u.y
#define ROTA(a, b, ang) a * cos(ang) - b * sin(ang)
#define ROTB(a, b, ang) a * sin(ang) + b * cos(ang)

#define MAXFLOATSIZE 30
#define MAXINTSIZE 30

void initObj(obj_t *Obj);
threeD_t *getRawVerts(list_t *VertInfo, unsigned int *VertNum);
void initCam(cam_t *Cam, threeD_t *RawVerts, unsigned int VertNum);
void moveCam(cam_t *Cam, char dir);
void getProjVerts(threeD_t *RawVerts, twoD_t *ProjVerts, unsigned int VertNum, cam_t Cam);
void convertToScrCoords(twoD_t *ProjVerts, unsigned int VertNum, unsigned int W, unsigned int H);
edge_t *getEdges(list_t *EdgeInfo, unsigned int *EdgeNum, list_t *FaceInfo);

#endif