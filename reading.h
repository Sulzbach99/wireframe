#ifndef __READING__
#define __READING__

#include "common.h"
#include "list.h"
#include <string.h>

#define MAXLINESIZE 50

void readFile(char *path, list_t *Verts, list_t *Faces);
void getThreeD(list_t *Verts);

#endif