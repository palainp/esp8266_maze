#ifndef BIN_HEAP_H
#define BIN_HEAP_H
#include "list.h"

#define MAX_SIZE 100000

typedef struct {
    int x,y;
    int gscore;
    int fscore; // the tree will minimize fscore
} t_tree_node;


#define NODE_TYPE   t_tree_node

NODE_TYPE* create_node(int x, int y, int gscore, int fscore);

// insert the item at the appropriate position
void enqueue(NODE_TYPE* a[], NODE_TYPE* data, size_t * n);

// deletes the max item and return
NODE_TYPE* dequeue(NODE_TYPE* a[], size_t * n);

// prints the heap
void print_heap(NODE_TYPE* a[], size_t n);

#endif
