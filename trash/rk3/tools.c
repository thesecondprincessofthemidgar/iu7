#define _GNU_SOURCE

#include "tools.h"
#include <string.h>

#define INITIAL_CAPACITY 10

// Проверка наличия точки в массиве
static int is_in(point_t p, point_t *points, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        if (p.x == points[i].x && p.y == points[i].y)
            return 1;
    }
    return 0;
}

// Чтение точек из файла с уникальностью
int read_points(const char *fname, point_t **points, size_t *size)
{
    *points = NULL;
    *size = 0;
    size_t capacity = 0;

    FILE *f = fopen(fname, "rt");
    if (!f)
    {
        fprintf(stderr, "Cannot open points file: %s\n", fname);
        return 1;
    }

    point_t new_point;
    while (fscanf(f, "%d %d", &new_point.x, &new_point.y) == 2)
    {
        if (!is_in(new_point, *points, *size))
        {
            // Добавляем точку
            if (*size == capacity)
            {
                capacity = (capacity == 0) ? INITIAL_CAPACITY : capacity * 2;
                point_t *tmp = realloc(*points, capacity * sizeof(point_t));
                if (!tmp)
                {
                    fclose(f);
                    free(*points);
                    fprintf(stderr, "Memory allocation error\n");
                    return 2;
                }
                *points = tmp;
            }
            (*points)[*size] = new_point;
            (*size)++;
        }
    }

    fclose(f);
    return 0;
}

// Компаратор для qsort: сначала по x, при равенстве по y
static int compare_points(const void *a, const void *b)
{
    const point_t *p1 = (const point_t *)a;
    const point_t *p2 = (const point_t *)b;
    if (p1->x < p2->x)
        return -1;
    else if (p1->x > p2->x)
        return 1;
    else
    {
        if (p1->y < p2->y)
            return -1;
        else if (p1->y > p2->y)
            return 1;
        else
            return 0;
    }
}

// Сортировка точек
void sort_points(point_t *base, size_t size)
{
    qsort(base, size, sizeof(point_t), compare_points);
}

// Чтение фигур
// Формат: каждая строка — одна фигура. В строке идут координаты точек подряд.
// Например: "1 2 3 4 5 6" => (1,2), (3,4), (5,6)
int read_figures(const char *fname, figure_t **figures, size_t *size)
{
    FILE *f = fopen(fname, "r");
    if (!f) return 1;

    // Подсчёт строк - каждая строка = одна фигура
    char line[MAX_LINE_SIZE];
    size_t lines = 0;
    while (fgets(line, MAX_LINE_SIZE, f) != NULL)
        lines++;
    rewind(f);

    figure_t *fgrs = malloc(sizeof(figure_t) * lines);
    if (!fgrs)
    {
        fclose(f);
        return 2;
    }

    for (size_t i = 0; i < lines; i++)
    {
        if (!fgets(line, MAX_LINE_SIZE, f))
        {
            // Если почему-то не считали строку, выходим
            free(fgrs);
            return 3;
        }

        // Подсчитаем количество чисел в строке
        int tmp;
        int count = 0;
        char *p = line;
        // Используем временный указатель для sscanf
        while (sscanf(p, "%d", &tmp) == 1)
        {
            count++;
            while (*p && (*p == ' ' || *p == '\t' || (*p >= '0' && *p <= '9') || *p == '-' || *p == '+'))
                p++;
        }

        // count - общее количество чисел
        int points_count = count / 2;
        fgrs[i].size = points_count;
        fgrs[i].points = malloc(points_count * sizeof(point_t));
        fgrs[i].binary_features = malloc(points_count * sizeof(int));

        // Теперь снова пройдем по строке и запишем числа
        p = line;
        for (int j = 0; j < points_count; j++)
        {
            int x, y;
            // Считываем x
            while (*p && (*p == ' ' || *p == '\t')) p++;
            sscanf(p, "%d", &x);
            while (*p && (*p == '-' || *p == '+' || (*p >= '0' && *p <= '9'))) p++;
            
            // Считываем y
            while (*p && (*p == ' ' || *p == '\t')) p++;
            sscanf(p, "%d", &y);
            while (*p && (*p == '-' || *p == '+' || (*p >= '0' && *p <= '9'))) p++;

            fgrs[i].points[j].x = x;
            fgrs[i].points[j].y = y;
        }
    }

    fclose(f);
    *figures = fgrs;
    *size = lines;
    return 0;
}

// Проставляем бинарные признаки
void check_binary_features(figure_t *figures, size_t fgrs_size, point_t *points, size_t points_size)
{
    for (size_t i = 0; i < fgrs_size; i++)
    {
        for (size_t j = 0; j < figures[i].size; j++)
        {
            if (is_in(figures[i].points[j], points, points_size))
                figures[i].binary_features[j] = 1;
            else
                figures[i].binary_features[j] = 0;
        }
    }
}

static void print_figure_features(figure_t figure)
{
    printf("( ");
    for (size_t i = 0; i < figure.size; i++)
        printf("%d ", figure.binary_features[i]);
    printf(")\n");
}

void print_all_figures(figure_t *figures, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%zu - ", i + 1);
        print_figure_features(figures[i]);
    }
}

// Освобождение памяти фигур
void free_figures(figure_t *figures, size_t size)
{
    if (!figures) return;
    for (size_t i = 0; i < size; i++)
    {
        free(figures[i].points);
        free(figures[i].binary_features);
    }
    free(figures);
}
