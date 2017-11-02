#include "vectors.h"

void createVector(vector_t *Vector, int nmembs)
{
    VECTORITEM(Vector) = Malloc(sizeof(void *) * nmembs);
    VECTORLENGTH(Vector) = 0;
}

void insertInVector(vector_t *Vector, void *ptr)
{
    VECTORITEM(Vector)[VECTORLENGTH(Vector)] = ptr;
    VECTORLENGTH(Vector)++;
}

char isInVector(vector_t *Vector, void *ptr)
{
    for (unsigned int i = 0; i < VECTORLENGTH(Vector); i++)
        if (VECTORITEM(Vector)[i] == ptr)
            return 1;

    return 0;
}