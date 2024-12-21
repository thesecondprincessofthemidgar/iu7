#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char *name;
    int repeats_count;
} name_t;

typedef struct
{
    name_t *array;
    size_t size;
} names_t;

int read_names(char *fname, names_t *names);
void sort_names(names_t *names);
int write_names(char *fname, names_t *names);
void free_names(names_t *names);

#endif
