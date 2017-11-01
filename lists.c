#include "lists.h"

/* Cria uma lista genérica com ponteiros First e Last iguais a NULL */

void createList(list_t *List)
{
    List = Malloc(sizeof(list_t));
    FIRSTCELL(List) = NULL;
    LASTCELL(List) = FIRSTCELL(List); // João 20:16
    List->Length = 0;
}

/********************************************************************/

/* Cria uma célula no final da lista dada                             /
** Se é a primeira célula da lista, o ponteiro First aponta para ela */

void createCell(list_t *List)
{
    NEXTCELL(LASTCELL(List)) = Malloc(sizeof(cell_t));
    LASTCELL(List) = NEXTCELL(LASTCELL(List));
    NEXTCELL(LASTCELL(List)) = NULL;
    List->Length++;

    if (!(FIRSTCELL(List)))
        FIRSTCELL(List) = LASTCELL(List);
}

/*********************************************************************/

/* Remove a primeira célula da lista dada */

void removeCell(list_t *List)
{
    if (!(FIRSTCELL(List)))
    {
        fprintf(stderr, "Error: Tried to remove cell from empty list\n");
        exit(EXIT_FAILURE);
    }

    cell_t *ptr;
    ptr = FIRSTCELL(List);
    FIRSTCELL(List) = NEXTCELL(FIRSTCELL(List));
    free(ptr);
    List->Length--;
}

/******************************************/