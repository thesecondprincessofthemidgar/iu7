#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "list_tools.h"


static int read_string(FILE *f, char **string)
{
    size_t buffer_size = 0;
    ssize_t len = 0;

    *string = NULL;
    len = getline(string, &buffer_size, f);

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
    List *list = (List *) malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void list_append_node(List *list, const char *word)
{
    Node *new_node = (Node *) malloc(sizeof(Node));
    new_node->data = strdup(word);
    new_node->prev = list->tail;
    new_node->next = NULL;

    if (list->tail)
        list->tail->next = new_node;
    else
        list->head = new_node;

    list->tail = new_node;
}

void reverse_word(char *word)
{
    int len = strlen(word);
    for (int i = 0; i < len / 2; i++)
    {
        char temp = word[i];
        word[i] = word[len - i - 1];
        word[len - i - 1] = temp;
    }
}

void reverse_list(List *list)
{
    Node *current = list->head;
    Node *temp = NULL;

    while (current)
    {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev;
    }

    if (temp)
    {
        list->tail = list->head;
        list->head = temp->prev;
    }
}

void reverse_sentence(List *list)
{
    Node *current = list->head;
    while (current)
    {
        reverse_word(current->data);
        current = current->next;
    }

    reverse_list(list);
}

int read_list(List *list, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
        return EIO;

    int error_code = 0;
    char *word = NULL;

    while ((error_code = read_string(file, &word)) == 0)
    {
        list_append_node(list, word);
        free(word);
        word = NULL;
    }

    if ((error_code == ENODATA || error_code == EIO) && feof(file))
        error_code = 0;

    if (word)
        free(word);

    fclose(file);
    return error_code;
}

int write_list(List *list, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
        return EIO;

    Node *current = list->head;
    while (current)
    {
        fprintf(file, "%s\n", current->data);
        current = current->next;
    }

    fclose(file);
    return 0;
}

void free_list(List *list)
{
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
