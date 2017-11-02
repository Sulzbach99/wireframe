#include "reading.h"

void readFile(char *path, list_t *Verts, list_t *Faces)
{
    FILE *OBJS;
    OBJS = fopen(path, "r");
    if (!OBJS)
    {
        fprintf(stderr, "Error: Could not load file\n");
        exit(EXIT_FAILURE):
    }

    createList(Verts);
    createList(Faces);

    char line[MAXLINESIZE];
    while (fgets(line, MAXLINESIZE, OBJS))
        if (line[0] == 'v' && line[1] == ' ')
        {
            createCell(Verts);
            LASTITEM(Verts) = Malloc(strlen(line) + 1);
            strcpy(LASTITEM(Verts), line);
        }
        else if (line[0] == 'f')
        {
            createCell(Faces);
            LASTITEM(Faces) = Malloc(strlen(line) + 1);
            strcpy(LASTITEM(Faces), line);
        }

    fclose(OBJS);
}