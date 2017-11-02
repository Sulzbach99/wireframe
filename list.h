#ifndef __LIST__
#define __LIST__

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

#define FIRSTITEM(list) list->Last->Item
#define LASTITEM(list) list->Last->Item
#define CURRENTITEM(cell) cell->Item

/***************************************/

cell_t *createList();
void createCell(list_t *List);
void removeCell(list_t *List);
void appendItem(cell_t *Cell, void *Addr);
cell_t *getLastCell(list_t *List);

#endif