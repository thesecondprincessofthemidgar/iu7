#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_SIZE 1024
#define MAX_POINTS  100

typedef struct
{
    int x;
    int y;
} point_t;

typedef struct
{
    point_t *points;
    int *binary_features;
    size_t size; // количество точек в фигуре
} figure_t;

int read_points(const char *fname, point_t **points, size_t *size);
int read_figures(const char *fname, figure_t **figures, size_t *size);
void sort_points(point_t *base, size_t size);
void check_binary_features(figure_t *figures, size_t fgrs_size, point_t *points, size_t points_size);
void print_all_figures(figure_t *figures, size_t size);
void free_figures(figure_t *figures, size_t size);

#endif