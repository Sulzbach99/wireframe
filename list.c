#include "list.h"

/* Cria uma lista genérica com ponteiros First e Last iguais a NULL */

cell_t *createList(list_t *List)
{
    List = Malloc(sizeof(list_t));
    FIRSTCELL(List) = NULL;
    LASTCELL(List) = NULL;
    LISTLENGTH(List) = 0;
    return List;
}

/********************************************************************/

/* Cria uma célula no final da lista dada                             /
** Se é a primeira célula da lista, o ponteiro First aponta para ela */

void createCell(list_t *List)
{
    if (!LASTCELL(List))
    {
        LASTCELL(List) = Malloc(sizeof(cell_t));
        FIRSTCELL(List) = LASTCELL(List);
    }
    else
    {
        NEXTCELL(LASTCELL(List)) = Malloc(sizeof(cell_t));
        LASTCELL(List) = NEXTCELL(LASTCELL(List));
    }

    NEXTCELL(LASTCELL(List)) = NULL;
    LISTLENGTH(List)++;
}

/*********************************************************************/

/* Remove a primeira célula da lista dada */

void removeCell(list_t *List)
{
    if (!FIRSTCELL(List))
    {
        fprintf(stderr, "Error: Tried to remove cell from empty list\n");
        exit(EXIT_FAILURE);
    }

    cell_t *ptr;
    ptr = FIRSTCELL(List);
    FIRSTCELL(List) = NEXTCELL(FIRSTCELL(List));
    free(ptr);
    LISTLENGTH(List)--;
}

/******************************************/

/* Faz o campo Item da célula dada apontar para o endereço dado */

void appendItem(cell_t *Cell, void *Addr)
{
    CURRENTITEM(Cell) = Addr;
}

/****************************************************************/

cell_t *getLastCell(list_t *List)
{
    return LASTCELL(List);
}