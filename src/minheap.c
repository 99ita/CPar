#include "minheap.h"


//Initialize the min heap
minHeap initMinHeap(int max) {
    minHeap hp;
    hp.size = 0;
    hp.max = max;
    hp.elems = (node*) calloc(max,sizeof(node));
    return hp;
}

//Print heap
void printHeap(minHeap* mh){
    printf("size: %d\n",mh->size);
    for(int i = 0; i < mh->size; i++){
        printf("%d ",mh->elems[i].elem);
    }
    printf("\n");
}

//Swap data within two nodes of the min heap using pointers
void swap(node *xp, node *yp){
    node temp = *xp;
    *xp = *yp;
    *yp = temp;
}


//Guarantees the heap condition is met
void heapify(minHeap *hp, int i){
    int smallest = (leftChild(i) < hp->size && hp->elems[leftChild(i)].elem < hp->elems[i].elem) ? leftChild(i) : i;
    if(rightChild(i) < hp->size && hp->elems[rightChild(i)].elem < hp->elems[smallest].elem) {
        smallest = rightChild(i);
    }
    if(smallest != i) {
        swap(&(hp->elems[i]), &(hp->elems[smallest]));
        heapify(hp, smallest);
    }
}


//Function to insert a node into the min heap
void pushHeap(minHeap *hp, int data, int thread) {
    if(hp->size == 0){hp->elems[0].elem = data; hp->elems[0].thread = thread; hp->size++; return;}
    if(hp->size == hp->max){
        hp->max *= 2;
        hp->elems = realloc(hp->elems, (hp->max) * sizeof(int));
    }
    
    int i = hp->size;
    hp->size++;

    while(i && data < hp->elems[parent(i)].elem){
        hp->elems[i].elem = hp->elems[parent(i)].elem;
        hp->elems[i].thread = hp->elems[parent(i)].thread;
        i = parent(i);
    }
    hp->elems[i].elem = data;
    hp->elems[i].thread = thread;
}

//Function to delete a node from the min heap
int popHeap(minHeap *hp, int* thread) {
    int r = hp->elems[0].elem;
    *thread = hp->elems[0].thread;
    if(hp->size){
        hp->size--;
        hp->elems[0].elem = hp->elems[hp->size].elem;
        hp->elems[0].thread = hp->elems[hp->size].thread;
        heapify(hp, 0);
    } else {
        printf("\nMin Heap is empty!\n");
        free(hp->elems);
    }
    return r;
}

