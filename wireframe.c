#include "graphics.h"

#define MAXPATHSIZE 50
#define MAXLINESIZE 50

void parseArgs(int argc, char *argv[], char *path)
{
    if (argc > 3)
    {
        fprintf(stderr, "Error: Too many arguments\n");
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '<')
        strcpy(path, argv[1]);
    else
        path = "stdin";
}

void readFile(char *path, list_t *Verts, list_t *Faces)
{
    FILE *OBJS;
    OBJS = fopen(path, "r");
    if (!OBJS)
    {
        fprintf(stderr, "Error: Could not load file\n");
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

    Object.RawVerts = getRawVerts(Object.VertInfo, &Object.VertNum);

    // Isso deveria ir em outro lugar...
    Object.ProjVerts = Malloc(sizeof(twoD_t) * Object.VertNum);

    threeD_t Camera;
    initCam(&Camera, Object.RawVerts, Object.VertNum);

    getProjVerts(Object.RawVerts, Object.ProjVerts, Object.VertNum, Camera);

    convertToScrCoords(Object.ProjVerts, Object.VertNum, WIDTH, HEIGHT);

    Object.Edges = getEdges(Object.EdgeInfo, &Object.EdgeNum, Object.FaceInfo);

    char status;
    initGraphics();
    while (status = plotObj(Object.ProjVerts, Object.Edges, Object.EdgeNum))
    {
        moveCam(&Camera, status);
        getProjVerts(Object.RawVerts, Object.ProjVerts, Object.VertNum, Camera);
        convertToScrCoords(Object.ProjVerts, Object.VertNum, WIDTH, HEIGHT);
    }
    killGraphics();

    exit(EXIT_SUCCESS);
}