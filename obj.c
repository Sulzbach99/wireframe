#include "obj.h"

/* Inicializa o objeto */

void initObj(obj_t *Obj)
{
    Obj->VertInfo = Malloc(sizeof(queue_t));
    Obj->FaceInfo = Malloc(sizeof(queue_t));
    Obj->EdgeInfo = Malloc(sizeof(queue_t));

    initQueue(Obj->VertInfo);
    initQueue(Obj->FaceInfo);
    initQueue(Obj->EdgeInfo);

    Obj->RawVerts = NULL;
    Obj->ProjVerts = NULL;
    Obj->Edges = NULL;

    Obj->VertNum = 0;
    Obj->EdgeNum = 0;
}

/***********************/

/* Faz uso de todas as informações queueadas em VertInfo para alocar e   /
** preencher um vetor de vértices tridimensionais. VertInfo é esvaziada /
** no processo                                                         */

threeD_t *getRawVerts(queue_t *VertInfo, unsigned int *VertNum)
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

    Cam->AngX = M_PI / 2;
    Cam->AngY = M_PI / 2;
}

/***********************/

/* Recalcula o ângulo da câmera */

void moveCam(cam_t *Cam, twoD_t dir)
{
    threeD_t Proj;
    double Ang = M_PI / 18;

    Proj.x = Cam->Coords.x;
    Proj.z = Cam->Coords.z;

    if (Cam->Coords.x != 0 || Cam->Coords.z != 0)
    {
        Cam->Coords.x = ROTA(Proj.x, Proj.z, -(Ang * dir.x));
        Cam->Coords.z = ROTB(Proj.x, Proj.z, -(Ang * dir.x));
    }

    Cam->AngX -= Ang * dir.x;

    Proj.x = Cam->Coords.x;
    Proj.y = Cam->Coords.y;
    Proj.z = Cam->Coords.z;

    Cam->Coords.x = ROTA(Proj.x, Proj.z, -Cam->AngX);
    Cam->Coords.z = ROTB(Proj.x, Proj.z, -Cam->AngX);

    Proj.x = Cam->Coords.x;
    Proj.z = Cam->Coords.z;

    Cam->Coords.x = ROTA(Proj.x, Proj.y, Ang * dir.y);
    Cam->Coords.y = ROTB(Proj.x, Proj.y, Ang * dir.y);

    Proj.x = Cam->Coords.x;

    Cam->Coords.x = ROTA(Proj.x, Proj.z, Cam->AngX);
    Cam->Coords.z = ROTB(Proj.x, Proj.z, Cam->AngX);

    Cam->AngY += Ang * dir.y;
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

        CurrentVert.x = ROTA(CurrentVert.x, CurrentVert.z, -Cam.AngX - M_PI / 2);
        CurrentVert.z = ROTB(CurrentVert.x, CurrentVert.z, -Cam.AngX - M_PI / 2);

        CurrentVert.y = ROTA(CurrentVert.y, CurrentVert.z, -Cam.AngY - M_PI / 2);
        CurrentVert.z = ROTB(CurrentVert.y, CurrentVert.z, -Cam.AngY - M_PI / 2);

        ProjVerts[i].x = CurrentVert.x;
        ProjVerts[i].y = CurrentVert.y;
    }

    printf("Cam.AngX: %f Cam.Angy: %f d: %f\n", Cam.AngX, Cam.AngY, d);
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

/* Preenche a queuea EdgeInfo à partir de FaceInfo, que é esvaziada no processo, /
** posteriormente, EdgeInfo é esvaziada, gerando o vetor Edges                 */

edge_t *getEdges(queue_t *EdgeInfo, unsigned int *EdgeNum, queue_t *FaceInfo)
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