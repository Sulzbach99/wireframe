#include "common.h"
#include "lists.h"
#include <strings.h>

void parseArgs(int argc, char *argv[], char *path)
{
    if (argc > 3)
    {
        fprintf(stderr, "Error: Too many arguments\n");
        exit(EXIT_FAILURE);
    }

    if (argv[1] != '<')
        strcpy(path, argv[1]);
    else
        path = "stdin";
}

int main(int argc, char *argv[])
{
    char *path;
    parseArgs(argc, argv, path);

    list_t *Verts, *Faces;
    readFile(path, Verts, Faces);

    exit(EXIT_SUCCESS)
}