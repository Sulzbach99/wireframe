#include "queue.h"

/* Cria uma fila genérica com ponteiros First e Last iguais a NULL */

void initQueue(queue_t *Queue)
{
    FIRSTCELL(Queue) = NULL;
    LASTCELL(Queue) = NULL;
    QUEUELENGTH(Queue) = 0;
}

/********************************************************************/

/* Cria uma célula no final da fila dada                             /
** Se é a primeira célula da fila, o ponteiro First aponta para ela */

void createCell(queue_t *Queue)
{
    if (!LASTCELL(Queue))
    {
        LASTCELL(Queue) = Malloc(sizeof(cell_t));
        FIRSTCELL(Queue) = LASTCELL(Queue);
    }
    else
    {
        NEXTCELL(LASTCELL(Queue)) = Malloc(sizeof(cell_t));
        LASTCELL(Queue) = NEXTCELL(LASTCELL(Queue));
    }

    NEXTCELL(LASTCELL(Queue)) = NULL;
    QUEUELENGTH(Queue)++;
}

/*********************************************************************/

/* Remove a primeira célula da fila dada */

void removeCell(queue_t *Queue)
{
    if (!FIRSTCELL(Queue))
    {
        fprintf(stderr, "Queue Error: Tried to remove cell from empty queue\n");
        exit(EXIT_FAILURE);
    }

    cell_t *ptr;
    ptr = FIRSTCELL(Queue);
    FIRSTCELL(Queue) = NEXTCELL(FIRSTCELL(Queue));
    free(ptr);
    QUEUELENGTH(Queue)--;

    if (!FIRSTCELL(Queue))
        LASTCELL(Queue) = NULL;
}

/******************************************/

/* Faz o campo Item da célula dada apontar para o endereço dado */

void appendItem(cell_t *Cell, void *Addr)
{
    CURRENTITEM(Cell) = Addr;
}

/*******************************************************/