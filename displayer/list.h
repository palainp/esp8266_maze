#ifndef LIST_H
#define LIST_H

typedef enum {
    false=0, true
} t_bool;

typedef struct _list_node{
    int x,y;
    int gscore;
    struct _list_node* next; // next in the list;
} t_list_node;

#define LIST_TYPE   t_list_node

LIST_TYPE* create(int x, int y, int gscore);

// insert the item at in the front of the list
void push_front(LIST_TYPE** l, LIST_TYPE* data);

// update an item if its gscore is better than the one stored, returns true if data is in the list
t_bool update_if_exists(LIST_TYPE* l, LIST_TYPE* data);

// prints the heap
void print_list(LIST_TYPE* l);

#endif
