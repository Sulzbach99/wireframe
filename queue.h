#ifndef __QUEUE__
#define __QUEUE__

#include "Malloc.h"

/* Definição de uma queuea genérica */

typedef struct cell {

    void *Item;
    struct cell *Next;

} cell_t;

typedef struct {

    unsigned int Length;
    cell_t *First;
    cell_t *Last;

} queue_t;

/***********************************/

/* Constantes para facilitar a sintaxe */

#define QUEUELENGTH(queue) queue->Length

#define FIRSTCELL(queue) queue->First
#define LASTCELL(queue) queue->Last
#define NEXTCELL(cell) cell->Next

#define FIRSTITEM(queue) queue->Last->Item
#define LASTITEM(queue) queue->Last->Item
#define CURRENTITEM(cell) cell->Item

/***************************************/

void initQueue(queue_t *Queue);
void insertInQueue(queue_t *Queue, void *Addr);
void *takeFromQueue(queue_t *Queue);

#endif