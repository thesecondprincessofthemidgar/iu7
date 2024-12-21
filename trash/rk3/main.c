#include <stdio.h>
#include <stdlib.h>
#include "tools.h"

int main(void)
{
    names_t names = { NULL, 0 };

    if (read_names("in.txt", &names))
    {
        return 1;
    }

    sort_names(&names);

    if (write_names("out.txt", &names))
    {
        free_names(&names);
        return 2;
    }

    free_names(&names);
    return 0;
}
