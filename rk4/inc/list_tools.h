#ifndef LIST_TOOLS_H
#define LIST_TOOLS_H

#include <stdio.h>

typedef struct Node
{
    char *data;
    struct Node *next;
} Node;

typedef struct List
{
    Node *head;
    Node *tail;
    int size;
} List;

List *init_list();
int read_list(List *list, const char *filename);
int split_list(List *list);
int write_list(List *list, const char *filename);
void free_list(List *list);

#endif
