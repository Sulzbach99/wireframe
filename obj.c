#include "obj.h"

void initTrig()
{
    for (unsigned int i = 0; i < 360; i++)
    {
        Sin[i] = sin(i * M_PI / 180);
        Cos[i] = cos(i * M_PI / 180);
    }
}

void rotate(float *x, float *y, float a, float b, short ang)
{
    while (ang < 0)
        ang += 360;

    while (ang >= 360)
        ang -= 360;

    *x = ROTA(a, b, ang);
    *y = ROTB(a, b, ang);
}

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

/* Inicializa a câmera */

void initCam(cam_t *Cam, threeD_t *RawVerts, unsigned int VertNum)
{
    float aux, Radius = NORM(RawVerts[0]);
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

    Cam->AngX = 90;
    Cam->AngY = 90;
}

/***********************/

/* Recalcula o ângulo da câmera */

void moveCam(cam_t *Cam, twoD_t dir)
{
    threeD_t Proj;

    Proj.x = Cam->Coords.x;
    Proj.z = Cam->Coords.z;

    rotate(&Cam->Coords.x, &Cam->Coords.z, Proj.x, Proj.z, (short) -dir.x);

    Cam->AngX -= dir.x;

    Proj.x = Cam->Coords.x;
    Proj.y = Cam->Coords.y;
    Proj.z = Cam->Coords.z;

    rotate(&Cam->Coords.x, &Cam->Coords.z, Proj.x, Proj.z, -Cam->AngX);

    Proj.x = Cam->Coords.x;
    Proj.z = Cam->Coords.z;

    rotate(&Cam->Coords.x, &Cam->Coords.y, Proj.x, Proj.y, (short) dir.y);

    Proj.x = Cam->Coords.x;

    rotate(&Cam->Coords.x, &Cam->Coords.z, Proj.x, Proj.z, Cam->AngX);

    Cam->AngY += dir.y;
}

/********************************/

/* Aloca espaço para o arranjo ProjVerts */

void allocProjVerts(twoD_t **ProjVerts, unsigned int VertNum)
{
    *ProjVerts = Malloc(sizeof(twoD_t) * VertNum);
}

/*****************************************/

/* Faz uso de todas as informações queueadas em VertInfo para alocar e   /
** preencher um vetor de vértices tridimensionais. VertInfo é esvaziada /
** no processo                                                         */

void getRawVerts(threeD_t **RawVerts, queue_t *VertInfo, unsigned int *VertNum)
{
    char *ptr, X[MAXFLOATSIZE], Y[MAXFLOATSIZE], Z[MAXFLOATSIZE];
    unsigned int i, j;
    *VertNum = QUEUELENGTH(VertInfo);
    *RawVerts = Malloc(sizeof(threeD_t) * (*VertNum));

    for (unsigned int k = 0; k < *VertNum; k++)
    {
        ptr = (char *) takeFromQueue(VertInfo);

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

        (*RawVerts)[k].x = atof(X);
        (*RawVerts)[k].y = atof(Y);
        (*RawVerts)[k].z = atof(Z);
    }
    free(VertInfo);
}

/***********************************************************************/

/* Aplica o cálculo de perspectiva, gerando um vetor de vértices bidimensionais /
** a partir do vetor de vértices tridimensionais                               */

void getProjVerts(threeD_t *RawVerts, twoD_t *ProjVerts, unsigned int VertNum, cam_t Cam)
{
    float d, lambda;
    threeD_t CurrentVert, ProjCenter;

    ProjCenter.x = -2 * Cam.Coords.x;
    ProjCenter.y = -2 * Cam.Coords.y;
    ProjCenter.z = -2 * Cam.Coords.z;

    d = -NORM(ProjCenter);

    for (unsigned int i = 0; i < VertNum; i++)
    {
        CurrentVert.x = RawVerts[i].x - Cam.Coords.x;
        CurrentVert.y = RawVerts[i].y - Cam.Coords.y;
        CurrentVert.z = RawVerts[i].z - Cam.Coords.z;

        lambda = d / (ProjCenter.x * CurrentVert.x + ProjCenter.y * CurrentVert.y + ProjCenter.z * CurrentVert.z );

        CurrentVert.x *= lambda;
        CurrentVert.y *= lambda;
        CurrentVert.z *= lambda;

        rotate(&CurrentVert.x, &CurrentVert.z, CurrentVert.x, CurrentVert.z, -Cam.AngX - 90);
        rotate(&CurrentVert.y, &CurrentVert.z, CurrentVert.y, CurrentVert.z, -Cam.AngY + 90);

        ProjVerts[i].x = CurrentVert.x;
        ProjVerts[i].y = CurrentVert.y;
    }

#ifdef __DEBUG__
    printf("AngX: %d AngY: %d\n", Cam.AngX, Cam.AngY);
    printf("\n");
#endif
}

/*******************************************************************************/

/* Preenche a queue EdgeInfo à partir de FaceInfo, que é esvaziada no processo, /
** posteriormente, EdgeInfo é esvaziada, gerando o vetor Edges                 */

static int compareEdges(const void *a, const void *b)
{
    edge_t *A = (edge_t *) a;
    edge_t *B = (edge_t *) b;

    if (A->Start < B->Start)
        return -1;

    if (A->Start > B->Start)
        return 1;

    if (A->End < B->End)
        return -1;

    if (A->End > B->End)
        return 1;

    return 0;
}

void getEdges(edge_t **Edges, queue_t *EdgeInfo, unsigned int *EdgeNum, queue_t *FaceInfo, twoD_t *ProjVerts)
{
    char *ptr, First[MAXINTSIZE], Prev[MAXINTSIZE], Next[MAXINTSIZE];
    unsigned int i, j, FaceNum = QUEUELENGTH(FaceInfo);
    twoD_t *Aux;
    edge_t *Edge;

    for (unsigned int k = 0; k < FaceNum; k++)
    {
        ptr = (char *) takeFromQueue(FaceInfo);

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
                Edge->Start = &ProjVerts[atoi(Prev) - 1];
                Edge->End = &ProjVerts[atoi(Next) - 1];

                if (Edge->Start > Edge->End)
                {
                    Aux = Edge->Start;
                    Edge->Start = Edge->End;
                    Edge->End = Aux;
                }

                insertInQueue(EdgeInfo, Edge);

                strcpy(Prev, Next);
            }
        }
        free(ptr);

        Edge = Malloc(sizeof(edge_t));
        Edge->Start = &ProjVerts[atoi(Prev) - 1];
        Edge->End = &ProjVerts[atoi(First) - 1];

        if (Edge->Start > Edge->End)
        {
            Aux = Edge->Start;
            Edge->Start = Edge->End;
            Edge->End = Aux;
        }

        insertInQueue(EdgeInfo, Edge);
    }
    free(FaceInfo);

    *EdgeNum = QUEUELENGTH(EdgeInfo);
    *Edges = Malloc(sizeof(edge_t) * (*EdgeNum));

    for (unsigned int k = 0; k < *EdgeNum; k++)
    {
        Edge = (edge_t *) takeFromQueue(EdgeInfo);

        (*Edges)[k].Start = Edge->Start;
        (*Edges)[k].End = Edge->End;

        free(Edge);
    }

    qsort(*Edges, *EdgeNum, sizeof(edge_t), compareEdges);

    Edge = Malloc(sizeof(edge_t));
    Edge->Start = (*Edges)[0].Start;
    Edge->End = (*Edges)[0].End;
    insertInQueue(EdgeInfo, Edge);
    for (unsigned int k = 1; k < *EdgeNum; k++)
        if (compareEdges(&(*Edges)[k-1], &(*Edges)[k]))
        {
            Edge = Malloc(sizeof(edge_t));
            Edge->Start = (*Edges)[k].Start;
            Edge->End = (*Edges)[k].End;
            insertInQueue(EdgeInfo, Edge);
        }
    free(*Edges);

    *EdgeNum = QUEUELENGTH(EdgeInfo);
    (*Edges) = Malloc(sizeof(edge_t) * (*EdgeNum));

    for (unsigned int k = 0; k < *EdgeNum; k++)
    {
        Edge = (edge_t *) takeFromQueue(EdgeInfo);

        (*Edges)[k].Start = Edge->Start;
        (*Edges)[k].End = Edge->End;

        free(Edge);
    }
    free(EdgeInfo);
}

/*******************************************************************************/

/* Converte as coordenadas cartesianas abstratas do vetor de vértices bidimensionais /
** para coordenadas de tela                                                         */

void convertToScrCoords(twoD_t *ProjVerts, unsigned int VertNum, unsigned int W, unsigned int H, float zoom)
{
    float Xmax, Xmin, Ymax, Ymin;
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

    float Xcen = (Xmax + Xmin) / 2;
    float Xdif = Xmax - Xmin;

    float Ycen = (Ymax + Ymin) / 2;
    float Ydif = Ymax - Ymin;

    float Scx = W / Xdif;
    float Scy = H / Ydif;

    float Scale;
    if (Scx < Scy)
        Scale = Scx;
    else
        Scale = Scy;

    for (unsigned int j = 0; j < VertNum; j++)
    {
        ProjVerts[j].x = ((ProjVerts[j].x - Xcen) * Scale * zoom) + W / 2;
        ProjVerts[j].y = ((ProjVerts[j].y - Ycen) * Scale * zoom) + H / 2;
    }
}

/************************************************************************************/