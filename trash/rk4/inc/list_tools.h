#ifndef LIST_TOOLS_H
#define LIST_TOOLS_H

typedef struct Node
{
    char *data;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct List
{
    Node *head;
    Node *tail;
} List;

List *init_list();

void reverse_sentence(List *list);
void reverse_list(List *list);

int read_list(List *list, const char *filename);
int write_list(List *list, const char *filename);
void free_list(List *list);

#endif
