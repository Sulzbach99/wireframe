#ifndef __VECTORS__
#define __VECTORS__

typedef struct {

    int Length;
    void *Item;

} vector_t;

#define VECTORLENGTH(vector) vector->Length
#define VECTORITEM(vector) vector->Item

#endif