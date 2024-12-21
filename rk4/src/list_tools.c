#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include "list_tools.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

static int read_string(FILE *f, char **string)
{
    size_t buf_size = 0;
    ssize_t len = getline(string, &buf_size, f);

    if (len == -1 || len == 1)
    {
        if (*string)
        {
            free(*string);
            *string = NULL;
        }
        return (feof(f) ? EIO : ENODATA);
    }

    if ((*string)[len - 1] == '\n')
        (*string)[len - 1] = '\0';

    return 0;
}

List *init_list()
{
    List *list = malloc(sizeof(List));
    if (!list)
        return NULL;

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

static void list_append_node(List *list, const char *str)
{
    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
        return;

    new_node->data = strdup(str); 
    new_node->next = NULL;

    if (list->size == 0)
    {
        list->head = new_node;
        list->tail = new_node;
    }
    else
    {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
}

int read_list(List *list, const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
        return EIO;

    int error_code = 0;
    char *tmp_line = NULL;

    while ((error_code = read_string(f, &tmp_line)) == 0)
    {
        list_append_node(list, tmp_line);
        free(tmp_line);
        tmp_line = NULL;
    }
    if ((error_code == EIO || error_code == ENODATA) && feof(f))
        error_code = 0;

    if (tmp_line)
        free(tmp_line);

    fclose(f);
    return error_code;
}

int split_list(List *list)
{
    Node *current = list->head;

    while (current)
    {
        Node *next_node = current->next;
        char *old_data = current->data;

        if (strchr(old_data, ' '))
        {
            char *buf = strdup(old_data);

            free(current->data);
            current->data = NULL;

            char *token = strtok(buf, " ");
            if (token)
            {
                current->data = strdup(token);
            }
            else
            {
                current->data = strdup("");
            }

            token = strtok(NULL, " ");
            while (token)
            {
                Node *new_node = malloc(sizeof(Node));
                new_node->data = strdup(token);
                new_node->next = next_node;

                current->next = new_node;
                current = new_node;
                list->size++;

                if (list->tail == next_node)
                    list->tail = new_node;

                token = strtok(NULL, " ");
            }

            free(buf);
        }
        else
        {
            size_t len = strlen(old_data);
            if (len > 1)
            {
                char *buf = strdup(old_data);

                free(current->data);
                current->data = NULL;

                current->data = malloc(2);
                current->data[0] = buf[0];
                current->data[1] = '\0';

                for (size_t i = 1; i < len; i++)
                {
                    Node *new_node = malloc(sizeof(Node));
                    new_node->data = malloc(2);
                    new_node->data[0] = buf[i];
                    new_node->data[1] = '\0';

                    new_node->next = next_node;
                    current->next = new_node;
                    current = new_node;
                    list->size++;

                    if (list->tail == next_node)
                        list->tail = new_node;
                }

                free(buf);
            }
        }

        current = next_node;
    }

    return 0;
}

int write_list(List *list, const char *filename)
{
    FILE *f = fopen(filename, "w");
    if (!f)
        return EIO;

    Node *cur = list->head;
    while (cur)
    {
        fprintf(f, "%s\n", cur->data);
        cur = cur->next;
    }

    fclose(f);
    return 0;
}

void free_list(List *list)
{
    if (!list)
        return;

    Node *current = list->head;
    while (current)
    {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(list);
}
