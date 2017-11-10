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

    Cam->AngXZ = M_PI / 2;
    Cam->AngY = M_PI / 2;
}

/***********************/

/* Recalcula o ângulo da câmera */
/* TODO:
** Implementar condicionais para projeções nulas
** Implementar e recalcular Xc e Yc */

void moveCam(cam_t *Cam, char dir)
{
    threeD_t Proj;
    double Ang = M_PI / 18;

    if (dir == 1) // LEFT
    {
        Proj.x = Cam->Coords.x;
        Proj.z = Cam->Coords.z;

        Cam->Coords.x = ROTA(Proj.x, Proj.z, Ang);
        Cam->Coords.z = ROTB(Proj.x, Proj.z, Ang);

        Cam->AngXZ += Ang;
    }
    else if (dir == 2) // RIGHT
    {
        Proj.x = Cam->Coords.x;
        Proj.z = Cam->Coords.z;

        Cam->Coords.x = ROTA(Proj.x, Proj.z, -Ang);
        Cam->Coords.z = ROTB(Proj.x, Proj.z, -Ang);

        Cam->AngXZ -= Ang;
    }
    else if (dir == 3) // UP
    {
        Proj.x = Cam->Coords.x;
        Proj.y = Cam->Coords.y;

        Cam->Coords.x = ROTA(Proj.x, Proj.y, Ang);
        Cam->Coords.y = ROTB(Proj.x, Proj.y, Ang);

        Proj.z = Cam->Coords.z;

        Cam->Coords.y = ROTA(Proj.y, Proj.z, Ang);
        Cam->Coords.z = ROTB(Proj.y, Proj.z, Ang);

        Cam->AngY -= Ang;
    }
    else if (dir == 4) // DOWN
    {
        Proj.x = Cam->Coords.x;
        Proj.y = Cam->Coords.y;

        Cam->Coords.x = ROTA(Proj.x, Proj.y, -Ang);
        Cam->Coords.y = ROTB(Proj.x, Proj.y, -Ang);

        Proj.z = Cam->Coords.z;

        Cam->Coords.y = ROTA(Proj.y, Proj.z, -Ang);
        Cam->Coords.z = ROTB(Proj.y, Proj.z, -Ang);

        Cam->AngY += Ang;
    }
}

/********************************/

/* Aplica o cálculo de perspectiva, gerando um vetor de vértices bidimensionais /
** a partir do vetor de vértices tridimensionais                               */

void getProjVerts(threeD_t *RawVerts, twoD_t *ProjVerts, unsigned int VertNum, cam_t Cam)
{
    double d, lambda;
    threeD_t CurrentVert, ProjCenter;

    ProjCenter.x = -2 * Cam.Coords.x;
    ProjCenter.y = -2 * Cam.Coords.y;
    ProjCenter.z = -2 * Cam.Coords.z;

    d = NORM(ProjCenter);

    for (unsigned int i = 0; i < VertNum; i++)
    {
        CurrentVert.x = RawVerts[i].x - Cam.Coords.x;
        CurrentVert.y = RawVerts[i].y - Cam.Coords.y;
        CurrentVert.z = RawVerts[i].z - Cam.Coords.z;

        lambda = d / (ProjCenter.x * CurrentVert.x + ProjCenter.y * CurrentVert.y + ProjCenter.z * CurrentVert.z );

        CurrentVert.x = CurrentVert.x * lambda - ProjCenter.x;
        CurrentVert.y = CurrentVert.y * lambda - ProjCenter.y;
        CurrentVert.z = CurrentVert.z * lambda - ProjCenter.z;

        if (CurrentVert.x != 0 || CurrentVert.z != 0)
        {
            CurrentVert.x = ROTA(CurrentVert.x, CurrentVert.z, -Cam.AngXZ + M_PI / 2);
            CurrentVert.z = ROTB(CurrentVert.x, CurrentVert.z, -Cam.AngXZ + M_PI / 2);

            CurrentVert.y = ROTA(CurrentVert.y, CurrentVert.z, -Cam.AngY - M_PI / 2);
            CurrentVert.z = ROTB(CurrentVert.y, CurrentVert.z, -Cam.AngY - M_PI / 2);
        }
        else
        {
            CurrentVert.y = ROTA(CurrentVert.y, CurrentVert.z, -Cam.AngY - M_PI / 2);
            CurrentVert.z = ROTB(CurrentVert.y, CurrentVert.z, -Cam.AngY - M_PI / 2);

            CurrentVert.x = ROTA(CurrentVert.x, CurrentVert.z, -Cam.AngXZ + M_PI / 2);
            CurrentVert.z = ROTB(CurrentVert.x, CurrentVert.z, -Cam.AngXZ + M_PI / 2);
        }

        ProjVerts[i].x = CurrentVert.x;
        ProjVerts[i].y = CurrentVert.y;
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
        ProjVerts[j].y = ((ProjVerts[j].y - Ycen) * Scale * 0.95) + H / 2;
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