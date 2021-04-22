// C implementation of a simple linked list
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

LIST_TYPE* create(int x, int y, int gscore)
{
    LIST_TYPE* node = (LIST_TYPE*)malloc(sizeof(LIST_TYPE));
    if (node!=NULL)
    {
        node->x = x;
        node->y = y;
        node->gscore = gscore;
        node->next = NULL;
    }
    return node;
}

// insert the item at in the front of the list
void push_front(LIST_TYPE** l, LIST_TYPE* data)
{
    data->next = *l;
    *l = data;
}

// update an item if its gscore is better than the one stored, returns true if data is in the list
t_bool update_if_exists(LIST_TYPE* l, LIST_TYPE* data)
{
    LIST_TYPE* tmp = l;
    while (!(tmp==NULL || (tmp->x==data->x && tmp->y==data->y)))
    {
        tmp=tmp->next;
    }
    if (tmp!=NULL && tmp->gscore > data->gscore)
    {
        tmp->gscore = data->gscore;
    }
    return tmp!=NULL;
}

// prints the heap
void print_list(LIST_TYPE* l)
{
    LIST_TYPE* tmp = l;
    while (tmp!=NULL)
    {
        printf("sss");
        tmp=tmp->next;
    }
}
