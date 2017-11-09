#include "obj.h"

/* Inicializa o objeto */

void initObj(obj_t *Obj)
{
    Obj->VertInfo = Malloc(sizeof(list_t));
    Obj->FaceInfo = Malloc(sizeof(list_t));
    Obj->EdgeInfo = Malloc(sizeof(list_t));

    initList(Obj->VertInfo);
    initList(Obj->FaceInfo);
    initList(Obj->EdgeInfo);

    Obj->RawVerts = NULL;
    Obj->ProjVerts = NULL;
    Obj->Edges = NULL;

    Obj->VertNum = 0;
    Obj->EdgeNum = 0;
}

/***********************/

/* Faz uso de todas as informações listadas em VertInfo para alocar e   /
** preencher um vetor de vértices tridimensionais. VertInfo é esvaziada /
** no processo                                                         */

threeD_t *getRawVerts(list_t *VertInfo, unsigned int *VertNum)
{
    *VertNum = VertInfo->Length;
    threeD_t *RawVerts = Malloc(sizeof(threeD_t) * (*VertNum));

    char *ptr, X[MAXFLOATSIZE], Y[MAXFLOATSIZE], Z[MAXFLOATSIZE];
    unsigned short i, j, k = 0;
    cell_t *Cell = VertInfo->First;
    while (Cell)
    {
        ptr = (char *) Cell->Item;

        i = 1;
        while (!(ptr[i] >= '0' && ptr[i] <= '9') && ptr[i] != '-')
            i++;

        j = 0;
        while (ptr[i] != ' ')
        {
            X[j] = ptr[i];
            i++;
            j++;
        }
        X[j] = '\0';

        while (!(ptr[i] >= '0' && ptr[i] <= '9') && ptr[i] != '-')
            i++;

        j = 0;
        while (ptr[i] != ' ')
        {
            Y[j] = ptr[i];
            i++;
            j++;
        }
        Y[j] = '\0';

        while (!(ptr[i] >= '0' && ptr[i] <= '9') && ptr[i] != '-')
            i++;

        j = 0;
        while ((ptr[i] >= '0' && ptr[i] <= '9') || ptr[i] == '.' || ptr[i] == '-' || ptr[i] == 'e')
        {
            Z[j] = ptr[i];
            i++;
            j++;
        }
        Z[j] = '\0';

        free(ptr);

        RawVerts[k].x = atof(X);
        RawVerts[k].y = atof(Y);
        RawVerts[k].z = atof(Z);

        k++;

        removeCell(VertInfo);
        Cell = VertInfo->First;
    }
    free(VertInfo);

    return RawVerts;
}

/***********************************************************************/

/* Inicializa a câmera */

void initCam(cam_t *Cam, threeD_t *RawVerts, unsigned int VertNum)
{
    double aux, Radius = NORM(RawVerts[0]);
    for (unsigned int i = 1; i < VertNum; i++)
    {
        aux = NORM(RawVerts[i]);
        if (aux > Radius)
            Radius = aux;
    }

    Radius *= 2;

    Cam->Coords.x = 0;
    Cam->Coords.y = 0;
    Cam->Coords.z = Radius;

    Cam->ProjXY.x = Cam->Coords.x;
    Cam->ProjXY.y = Cam->Coords.y;
    Cam->ProjXY.z = 0;

    Cam->ProjYZ.x = 0;
    Cam->ProjYZ.y = Cam->Coords.y;
    Cam->ProjYZ.z = Cam->Coords.z;

    Cam->ProjXZ.x = Cam->Coords.x;
    Cam->ProjXZ.y = 0;
    Cam->ProjXZ.z = Cam->Coords.z;

    Cam->Xc.x = 1;
    Cam->Xc.y = 0;
    Cam->Xc.z = 0;

    Cam->Yc.x = 0;
    Cam->Yc.y = 1;
    Cam->Yc.z = 0;
}

/***********************/

/* Recalcula o ângulo da câmera */
/* TODO:
** Implementar condicionais para projeções nulas
** Implementar e recalcular Xc e Yc */

void moveCam(cam_t *Cam, char dir)
{
    if (dir == 1) // LEFT
    {
        Cam->Coords.x = ROTA(Cam->ProjXZ.x, Cam->ProjXZ.z, M_PI / 18);
        Cam->Coords.z = ROTB(Cam->ProjXZ.x, Cam->ProjXZ.z, M_PI / 18);

        Cam->ProjXZ.x = Cam->Coords.x;
        Cam->ProjXZ.z = Cam->Coords.z;

        Cam->Xc.x = ROTA(Cam->ProjXZ.x, Cam->ProjXZ.z, M_PI);
        Cam->Xc.z = ROTB(Cam->ProjXZ.x, Cam->ProjXZ.z, M_PI);

        Cam->Yc.x = ORTX(Cam->Coords, Cam->Xc);
        Cam->Yc.y = ORTY(Cam->Coords, Cam->Xc);
        Cam->Yc.z = ORTZ(Cam->Coords, Cam->Xc);

        if (Cam->Yc.z < 0)
        {
            Cam->Yc.x *= -1;
            Cam->Yc.y *= -1;
            Cam->Yc.z *= -1;
        }
    }
    else if (dir == 2) // RIGHT
    {
        Cam->Coords.x = ROTA(Cam->ProjXZ.x, Cam->ProjXZ.z, - M_PI / 18);
        Cam->Coords.z = ROTB(Cam->ProjXZ.x, Cam->ProjXZ.z, - M_PI / 18);

        Cam->ProjXZ.x = Cam->Coords.x;
        Cam->ProjXZ.z = Cam->Coords.z;

        Cam->Xc.x = ROTA(Cam->ProjXZ.x, Cam->ProjXZ.z, M_PI);
        Cam->Xc.z = ROTB(Cam->ProjXZ.x, Cam->ProjXZ.z, M_PI);

        Cam->Yc.x = ORTX(Cam->Coords, Cam->Xc);
        Cam->Yc.y = ORTY(Cam->Coords, Cam->Xc);
        Cam->Yc.z = ORTZ(Cam->Coords, Cam->Xc);

        if (Cam->Yc.z < 0)
        {
            Cam->Yc.x *= -1;
            Cam->Yc.y *= -1;
            Cam->Yc.z *= -1;
        }
    }
    else if (dir == 3) // UP
    {
        Cam->Coords.x = ROTA(Cam->ProjXY.x, Cam->ProjXY.y, M_PI / 18);
        Cam->Coords.y = ROTB(Cam->ProjXY.x, Cam->ProjXY.y, M_PI / 18);

        Cam->ProjXY.x = Cam->Coords.x;
        Cam->ProjXY.y = Cam->Coords.y;

        Cam->Coords.y = ROTA(Cam->ProjYZ.y, Cam->ProjYZ.z, M_PI / 18);
        Cam->Coords.z = ROTB(Cam->ProjYZ.y, Cam->ProjYZ.z, M_PI / 18);

        Cam->ProjYZ.y = Cam->Coords.y;
        Cam->ProjYZ.z = Cam->Coords.z;

        Cam->Yc.x = ORTX(Cam->Coords, Cam->Xc);
        Cam->Yc.y = ORTY(Cam->Coords, Cam->Xc);
        Cam->Yc.z = ORTZ(Cam->Coords, Cam->Xc);

        if (Cam->Yc.z < 0)
        {
            Cam->Yc.x *= -1;
            Cam->Yc.y *= -1;
            Cam->Yc.z *= -1;
        }
    }
    else if (dir == 4) // DOWN
    {
        Cam->Coords.x = ROTA(Cam->ProjXY.x, Cam->ProjXY.y, - M_PI / 18);
        Cam->Coords.y = ROTB(Cam->ProjXY.x, Cam->ProjXY.y, - M_PI / 18);

        Cam->ProjXY.x = Cam->Coords.x;
        Cam->ProjXY.y = Cam->Coords.y;

        Cam->Coords.y = ROTA(Cam->ProjYZ.y, Cam->ProjYZ.z, - M_PI / 18);
        Cam->Coords.z = ROTB(Cam->ProjYZ.y, Cam->ProjYZ.z, - M_PI / 18);

        Cam->ProjYZ.y = Cam->Coords.y;
        Cam->ProjYZ.z = Cam->Coords.z;

        Cam->Yc.x = ORTX(Cam->Coords, Cam->Xc);
        Cam->Yc.y = ORTY(Cam->Coords, Cam->Xc);
        Cam->Yc.z = ORTZ(Cam->Coords, Cam->Xc);

        if (Cam->Yc.z < 0)
        {
            Cam->Yc.x *= -1;
            Cam->Yc.y *= -1;
            Cam->Yc.z *= -1;
        }
    }
}

/********************************/

/* Aplica o cálculo de perspectiva, gerando um vetor de vértices bidimensionais /
** a partir do vetor de vértices tridimensionais                               */

void getProjVerts(threeD_t *RawVerts, twoD_t *ProjVerts, unsigned int VertNum, cam_t Cam)
{
    double d, lambda, Ang1, Ang2;
    threeD_t CurrentVert, ProjCenter, ProjYc;

    ProjCenter.x = -2 * Cam.Coords.x;
    ProjCenter.y = -2 * Cam.Coords.y;
    ProjCenter.z = -2 * Cam.Coords.z;

    d = NORM(ProjCenter);

    Ang1 = - (acos(Cam.ProjXZ.x / NORM(Cam.ProjXZ)) + M_PI / 2);

    ProjYc.x = Cam.Yc.x;
    ProjYc.z = Cam.Yc.z;

    Cam.Yc.x = ROTA(Cam.Yc.x, ProjYc.z, Ang1);
    Cam.Yc.z = ROTB(Cam.Yc.x, ProjYc.z, Ang1);

    Ang2 = - acos(Cam.Yc.y / NORM(Cam.Yc));

    for (unsigned int i = 0; i < VertNum; i++)
    {
        CurrentVert.x = RawVerts[i].x - Cam.Coords.x;
        CurrentVert.y = RawVerts[i].y - Cam.Coords.y;
        CurrentVert.z = RawVerts[i].z - Cam.Coords.z;

        lambda = d / (-Cam.Coords.x * CurrentVert.x - Cam.Coords.y * CurrentVert.y - Cam.Coords.z * CurrentVert.z );

        CurrentVert.x *= lambda;
        CurrentVert.y *= lambda;
        CurrentVert.z *= lambda;

        CurrentVert.x = ROTA(CurrentVert.x, CurrentVert.z, Ang1);
        CurrentVert.z = ROTB(CurrentVert.x, CurrentVert.z, Ang1);

        CurrentVert.y = ROTA(CurrentVert.y, CurrentVert.z, Ang2);
        CurrentVert.z = ROTB(CurrentVert.y, CurrentVert.z, Ang2);
/*
        if (CurrentVert.z == 0)
        {
*/
            ProjVerts[i].x = CurrentVert.x;
            ProjVerts[i].y = CurrentVert.y;
/*
        }
        else
        {
            fprintf(stderr, "Calculation error\n");
            exit(EXIT_FAILURE);
        }
*/
    }
}

/*******************************************************************************/

/* Converte as coordenadas cartesianas abstratas do vetor de vértices bidimensionais /
** para coordenadas de tela                                                         */

void convertToScrCoords(twoD_t *ProjVerts, unsigned int VertNum, unsigned int W, unsigned int H)
{
    double Xmax, Xmin, Ymax, Ymin;
    Xmax = Xmin = ProjVerts[0].x;
    Ymax = Ymin = ProjVerts[0].y;
    for (unsigned int i = 1; i < VertNum; i++)
    {
        if (ProjVerts[i].x > Xmax)
            Xmax = ProjVerts[i].x;

        if (ProjVerts[i].x < Xmin)
            Xmin = ProjVerts[i].x;

        if (ProjVerts[i].y > Ymax)
            Ymax = ProjVerts[i].y;

        if (ProjVerts[i].y < Ymin)
            Ymin = ProjVerts[i].y;
    }

    double Xcen = (Xmax + Xmin) / 2;
    double Xdif = Xmax - Xmin;

    double Ycen = (Ymax + Ymin) / 2;
    double Ydif = Ymax - Ymin;

    double Scx = W / Xdif;
    double Scy = H / Ydif;

    double Scale;
    if (Scx < Scy)
        Scale = Scx;
    else
        Scale = Scy;

    for (unsigned int j = 0; j < VertNum; j++)
    {
        ProjVerts[j].x = ((ProjVerts[j].x - Xcen) * Scale * 0.95) + W / 2;
        ProjVerts[j].y = -((ProjVerts[j].y - Ycen) * Scale * 0.95) + H / 2;
    }
}

/************************************************************************************/

/* Preenche a lista EdgeInfo à partir de FaceInfo, que é esvaziada no processo, /
** posteriormente, EdgeInfo é esvaziada, gerando o vetor Edges                 */

edge_t *getEdges(list_t *EdgeInfo, unsigned int *EdgeNum, list_t *FaceInfo)
{
    char *ptr, First[MAXINTSIZE], Prev[MAXINTSIZE], Next[MAXINTSIZE];
    unsigned int i, j;
    edge_t *Edge;
    cell_t *Cell = FaceInfo->First;
    while (Cell)
    {
        ptr = (char *) Cell->Item;

        i = 1;
        while (ptr[i] < '0' || ptr[i] > '9')
            i++;

        j = 0;
        while (ptr[i] >= '0' && ptr[i] <= '9')
        {
            First[j] = ptr[i];
            i++;
            j++;
        }
        First[j] = '\0';

        strcpy(Prev, First);

        while (ptr[i] != '\0')
        {
            while (ptr[i] != ' ' && ptr[i] != '\0')
                i++;

            while ((ptr[i] < '0' || ptr[i] > '9') && ptr[i] != '\0')
                i++;

            j = 0;
            while (ptr[i] >= '0' && ptr[i] <= '9')
            {
                Next[j] = ptr[i];
                i++;
                j++;
            }
            if (j)
            {
                Next[j] = '\0';

                Edge = Malloc(sizeof(edge_t));
                Edge->Start = atoi(Prev) - 1;
                Edge->End = atoi(Next) - 1;

                createCell(EdgeInfo);
                appendItem(EdgeInfo->Last, Edge);

                strcpy(Prev, Next);
            }
        }
        free(ptr);

        Edge = Malloc(sizeof(edge_t));
        Edge->Start = atoi(Prev) - 1;
        Edge->End = atoi(First) - 1;

        createCell(EdgeInfo);
        appendItem(EdgeInfo->Last, Edge);

        removeCell(FaceInfo);
        Cell = FaceInfo->First;
    }
    free(FaceInfo);

    *EdgeNum = EdgeInfo->Length;
    edge_t *Edges = Malloc(sizeof(edge_t) * (*EdgeNum));

    i = 0;
    Cell = EdgeInfo->First;
    while (Cell)
    {
        Edge = (edge_t *) Cell->Item;

        Edges[i].Start = Edge->Start;
        Edges[i].End = Edge->End;

        free(Edge);

        i++;

        removeCell(EdgeInfo);
        Cell = EdgeInfo->First;
    }
    free(EdgeInfo);

    return Edges;
}

/*******************************************************************************/