#include <stdio.h>
#include <stdlib.h>
#include "list_tools.h"

#define FILENAME_IN  "in.txt"
#define FILENAME_OUT "out.txt"

int main(void)
{
    List *list = init_list();

    if (read_list(list, FILENAME_IN) != 0)
    {
        free_list(list);
        return 1;
    }

    if (split_list(list) != 0)
    {
        free_list(list);
        return 2;
    }

    if (write_list(list, FILENAME_OUT) != 0)
    {
        free_list(list);
        return 3;
    }

    free_list(list);
    return 0;
}
