// https://algorithmtutor.com/Data-Structures/Tree/Priority-Queues/
// C implementation of a max priority queue
#include <stdio.h>
#include <stdlib.h>
#include "bin_heap.h"

NODE_TYPE* create_node(int x, int y, int gscore, int fscore)
{
    NODE_TYPE* node = (NODE_TYPE*)malloc(sizeof(NODE_TYPE));
    if (node!=NULL)
    {
        node->x = x;
        node->y = y;
        node->gscore = gscore;
        node->fscore = fscore;
    }
    return node;
}

// returns the index of the parent node
size_t parent(size_t i) {
        return (i - 1) / 2;
}

// return the index of the left child 
size_t left_child(size_t i) {
        return 2 * i + 1;
}

// return the index of the right child 
size_t right_child(size_t i) {
        return 2 * i + 2;
}

void swap(NODE_TYPE* * x, NODE_TYPE* * y) {
        NODE_TYPE* temp = * x;
        * x = * y;
        * y = temp;
}

// return true if a < b
t_bool is_min(NODE_TYPE* a, NODE_TYPE* b) {
    return a->fscore < b->fscore;
}

// return true if a > b
t_bool is_max(NODE_TYPE* a, NODE_TYPE* b) {
    return a->fscore > b->fscore;
}

// insert the item at the appropriate position
void enqueue(NODE_TYPE* a[], NODE_TYPE* data, size_t * n) {
        if ( * n >= MAX_SIZE) {
                printf("%s\n", "The heap is full. Cannot insert");
                return;
        }
        //printf("enqueue %d %d %d %d\n", data->x, data->y, data->gscore, data->fscore);

        // first insert the time at the last position of the array 
        // and move it up
        a[ * n] = data;
        * n = * n + 1;

        // move up until the heap property satisfies
        size_t i = * n - 1;
        while (i != 0 && is_max(a[parent(i)],a[i])) {
                swap( & a[parent(i)], & a[i]);
                i = parent(i);
        }
}

// moves the item at position i of array a
// into its appropriate position
void max_heapify(NODE_TYPE* a[], size_t i, size_t n) {
        // find left child node
        size_t left = left_child(i);

        // find right child node
        size_t right = right_child(i);

        // find the largest among 3 nodes
        size_t largest = i;

        // check if the left node is larger than the current node
        if (left <= n && is_min(a[left],a[largest])) {
                largest = left;
        }

        // check if the right node is larger than the current node
        if (right <= n && is_min(a[right],a[largest])) {
                largest = right;
        }

        // swap the largest node with the current node 
        // and repeat this process until the current node is larger than 
        // the right and the left node
        if (largest != i) {
                swap(&(a[i]), &(a[largest]));
                //NODE_TYPE* temp = a[i];
                //a[i] = a[largest];
                //a[largest] = temp;
                max_heapify(a, largest, n);
        }

}

// returns the  maximum item of the heap
NODE_TYPE* get_max(NODE_TYPE* a[]) {
        return a[0];
}

// deletes the max item and return
NODE_TYPE* dequeue(NODE_TYPE* a[], size_t * n) {
        NODE_TYPE* max_item = a[0];
        //printf("dequeue %d %d %d %d\n", max_item->x, max_item->y, max_item->gscore, max_item->fscore);

        // replace the first item with the last item
        a[0] = a[ * n - 1];
        * n = * n - 1;

        // maintain the heap property by heapifying the 
        // first item
        max_heapify(a, 0, * n);
        return max_item;
}

// prints the heap
void print_heap(NODE_TYPE* a[], size_t n) {
        size_t i;
        for (i = 0; i < n; i++) {
                printf("%d,%d (%d)\n", a[i]->x, a[i]->y, a[i]->gscore);
        }
        printf("\n");
}
