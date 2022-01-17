#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define leftChild(x) 2 * x + 1
#define rightChild(x) 2 * x + 2
#define parent(x) (x - 1) / 2

typedef struct node{
    int elem;
    int thread;
} node;

typedef struct minHeap {
    int size;
    int max;
    node* elems;
} minHeap;

//Initialize the min heap
minHeap initMinHeap(int max);

//Print heap
void printHeap(minHeap* mh);

//Swap data within two nodes of the min heap using pointers
void swap(node *xp, node *yp);

//Guarantees the heap condition is met
void heapify(minHeap *hp, int i);

//Function to insert a node into the min heap
void pushHeap(minHeap *hp, int data, int thread);

//Function to delete a node from the min heap
int popHeap(minHeap *hp, int* thread);