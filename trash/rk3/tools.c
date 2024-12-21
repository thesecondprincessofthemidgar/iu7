#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include "tools.h"
#include <string.h>

static int compare_names(const void *a, const void *b)
{
    const name_t *aa = (const name_t *)a;
    const name_t *bb = (const name_t *)b;
    return strcmp(aa->name, bb->name);
}

static int find_name(const names_t *names, const char *name)
{
    for (size_t i = 0; i < names->size; i++)
        if (strcmp(names->array[i].name, name) == 0)
            return i;

    return -1;
}

static int add_name(names_t *names, char *buf)
{
    int pos = find_name(names, buf);
    if (pos >= 0)
    {
        names->array[pos].repeats_count++;
        return 0;
    }

    name_t *tmp = realloc(names->array, (names->size + 1) * sizeof(name_t));
    if (!tmp)
    {
        return 1;
    }

    names->array = tmp;

    names->array[names->size].name = strdup(buf);
    if (!names->array[names->size].name)
        return 1;

    names->array[names->size].repeats_count = 1;
    names->size++;
    
    return 0;
}

int read_names(char *fname, names_t *names)
{
    FILE *f = fopen(fname, "r");
    if (!f)
        return 1;

    names->array = NULL;
    names->size = 0;

    char *line = NULL;
    size_t buffer_size = 0;
    ssize_t len;

    while ((len = getline(&line, &buffer_size, f)) != -1)
    {
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';

        if (add_name(names, line) != 0)
        {
            fclose(f);
            free(line);
            return -1;
        }
    }

    fclose(f);
    free(line);

    return 0;
}

void sort_names(names_t *names)
{
    qsort(names->array, names->size, sizeof(name_t), compare_names);
}

int write_names(char *fname, names_t *names)
{
    FILE *f = fopen(fname, "w");
    if (!f)
        return 1;

    for (size_t i = 0; i < names->size; i++)
        fprintf(f, "%s: %d\n", names->array[i].name, names->array[i].repeats_count);

    fclose(f);
    
    return 0;
}

void free_names(names_t *names)
{
    if (!names || !names->array)
        return;

    for (size_t i = 0; i < names->size; i++)
        free(names->array[i].name);

    free(names->array);
    names->array = NULL;
    names->size  = 0;
}
