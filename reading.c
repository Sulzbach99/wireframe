#include "reading.h"

void readFile(char *path, list_t *Verts, list_t *Faces)
{
    FILE *OBJS;
    OBJS = fopen(path, "r");
    if (!OBJS)
    {
        fprintf(stderr, "Error: Could not load file\n");
        exit(EXIT_FAILURE);
    }

    initList(Verts);
    initList(Faces);

    char line[MAXLINESIZE], *ptr;
    while (fgets(line, MAXLINESIZE, OBJS))
        if (line[0] == 'v' && line[1] == ' ')
        {
            createCell(Verts);
            ptr = Malloc(strlen(line) + 1);
            strcpy(ptr, line);
            appendItem(LASTCELL(Verts), ptr);
        }
        else if (line[0] == 'f')
        {
            createCell(Faces);
            ptr = Malloc(strlen(line) + 1);
            strcpy(ptr, line);
            appendItem(LASTCELL(Faces), ptr);
        }

    fclose(OBJS);
}

void getVerts(list_t *Verts)
{
    cell_t *Cell = FIRSTCELL(Verts);
    threeD_t *Coord;
    char *ptr, X[MAXLINESIZE], Y[MAXLINESIZE], Z[MAXLINESIZE];
    unsigned short i = 2, j;

    while (Cell)
    {
        ptr = (char *) CURRENTITEM(Cell);

        j = 0;
        while (ptr[i] != ' ')
        {
            X[j] = ptr[i];
            i++;
            j++;
        }
        X[j] = '\0';
        i++;

        j = 0;
        while (ptr[i] != ' ')
        {
            Y[j] = ptr[i];
            i++;
            j++;
        }
        Y[j] = '\0';
        i++;

        j = 0;
        while (ptr[i] != ' ' && ptr[i] !=  '\n')
        {
            Z[j] = ptr[i];
            i++;
            j++;
        }
        Z[j] = '\0';

        free(ptr);

        Coord = Malloc(sizeof(threeD_t));

        GETX(Coord) = atof(X);
        GETY(Coord) = atof(Y);
        GETZ(Coord) = atof(Z);

        appendItem(Cell, Coord);

        Cell = NEXTCELL(Cell);
    }
}