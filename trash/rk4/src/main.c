#include "list_tools.h"

#define FILENAME_IN "in.txt"
#define FILENAME_OUT "out.txt"

int main(void)
{
    List *list = init_list();

    int error_code = 0;
    if ((error_code = read_list(list, FILENAME_IN)))
        return error_code;

    reverse_sentence(list);

    if ((error_code = write_list(list, FILENAME_OUT)))
        return error_code;

    free_list(list);

    return 0;
}
