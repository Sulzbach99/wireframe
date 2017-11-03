#include "obj.h"

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

twoD_t *putInPerspective(list_t *Input, twoD_t *Output)
{
    cell_t *Cell;
    threeD_t *ptr;
    unsigned int count = 0;

    Output = Malloc(sizeof(twoD_t) * LISTLENGTH(Input));

    while (Cell)
    {
        Cell = FIRSTCELL(Input);

        ptr = (threeD_t *) CURRENTITEM(Cell);

        GETX(Output[count]) = 
    }
}