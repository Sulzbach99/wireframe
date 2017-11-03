#include "Malloc.h"

/* Implementação de um malloc com verificação */

void *Malloc(int size)
{
    void *ptr;
    ptr = malloc(size);
    if (!ptr)
    {
        fprintf(stderr, "Malloc error\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

/**********************************************/