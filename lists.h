#ifndef __LISTS__
#define __LISTS__

#include "common.h"

/* Definição de uma lista genérica */

typedef struct cell {

    void *Item;
    struct cell *Next;

} cell_t;

typedef struct {

    int Length;
    cell_t *First;
    cell_t *Last;

} list_t;

/***********************************/

/* Constantes para facilitar a sintaxe */

#define LISTLENGTH(list) list->Length
#define FIRSTCELL(list) list->First
#define LASTCELL(list) list->Last
#define NEXTCELL(cell) cell->Next

/***************************************/

void createList(list_t *List);
void createCell(list_t *List);

#endif