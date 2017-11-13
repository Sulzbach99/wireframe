#include "graphics.h"

#define MAXPATHSIZE 50
#define MAXLINESIZE 50

void parseArgs(int argc, char *argv[], char *path)
{
    if (argc > 1)
        strcpy(path, argv[1]);
    else
        strcpy(path, "");
}

void readFile(char *path, queue_t *Verts, queue_t *Faces)
{
    FILE *OBJS;

    if (path[0])
        OBJS = fopen(path, "r");
    else
        OBJS = stdin;

    if (!OBJS)
    {
        fprintf(stderr, "Reading Error: Could not load file\n");
        exit(EXIT_FAILURE);
    }

    char line[MAXLINESIZE], *ptr;
    while (fgets(line, MAXLINESIZE, OBJS))
        if (line[0] == 'v' && line[1] == ' ')
        {
            createCell(Verts);
            ptr = Malloc(strlen(line) + 1);
            strcpy(ptr, line);
            appendItem(Verts->Last, ptr);
        }
        else if (line[0] == 'f')
        {
            createCell(Faces);
            ptr = Malloc(strlen(line) + 1);
            strcpy(ptr, line);
            appendItem(Faces->Last, ptr);
        }

    fclose(OBJS);
}

int main(int argc, char *argv[])
{
    char path[MAXPATHSIZE];
    parseArgs(argc, argv, path);

    obj_t Object;
    initObj(&Object);
    readFile(path, Object.VertInfo, Object.FaceInfo);

    getRawVerts(&Object.RawVerts, Object.VertInfo, &Object.VertNum);

    cam_t Camera;
    initCam(&Camera, Object.RawVerts, Object.VertNum);

#ifdef __DEBUG__
    printf("Cam: X:%.32f Y:%.32f Z:%.32f\n", Camera.Coords.x, Camera.Coords.y, Camera.Coords.z);
#endif

    allocProjVerts(&Object.ProjVerts, Object.VertNum);
    getProjVerts(Object.RawVerts, Object.ProjVerts, Object.VertNum, Camera);
    double zoom = 0.95;
    convertToScrCoords(Object.ProjVerts, Object.VertNum, WIDTH, HEIGHT, zoom);

    getEdges(&Object.Edges, Object.EdgeInfo, &Object.EdgeNum, Object.FaceInfo, Object.ProjVerts);

    twoD_t dir;
    char status;
    initGraphics();
    status = plotObj(Object.Edges, Object.EdgeNum, &Camera.Coords, &dir, &zoom);
    while (status)
    {
        moveCam(&Camera, dir);

#ifdef __DEBUG__
        printf("Cam: X:%.32f Y:%.32f Z:%.32f\n", Camera.Coords.x, Camera.Coords.y, Camera.Coords.z);
#endif

        getProjVerts(Object.RawVerts, Object.ProjVerts, Object.VertNum, Camera);
        convertToScrCoords(Object.ProjVerts, Object.VertNum, WIDTH, HEIGHT, zoom);
        status = plotObj(Object.Edges, Object.EdgeNum, &Camera.Coords, &dir, &zoom);
    }
    killGraphics();

    exit(EXIT_SUCCESS);
}