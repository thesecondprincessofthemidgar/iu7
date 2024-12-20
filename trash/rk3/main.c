#include "tools.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Использование: %s <файл с точками> <файл с фигурами>\n", argv[0]);
        return 1;
    }

    point_t *points = NULL;
    size_t points_size = 0;
    int error_code;

    // Читаем точки
    if ((error_code = read_points(argv[1], &points, &points_size)) != 0)
        return error_code;

    // Сортируем точки
    sort_points(points, points_size);

    figure_t *figures = NULL;
    size_t figures_size = 0;

    // Читаем фигуры
    if ((error_code = read_figures(argv[2], &figures, &figures_size)) != 0)
    {
        free(points);
        return error_code;
    }

    // Проставляем бинарные признаки
    check_binary_features(figures, figures_size, points, points_size);

    // Печатаем результаты
    print_all_figures(figures, figures_size);

    // Освобождаем память
    free_figures(figures, figures_size);
    free(points);

    return 0;
}
