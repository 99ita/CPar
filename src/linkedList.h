#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


//------------------Utilities------------------

// Linked list structure to represent a bucket
struct LList{
    int size;
    struct Node *head;
};

// Node of a linked list
struct Node{
    int value;
    struct Node *next;
};
 
// Add node to head of linked list 
void push(struct LList** list, int value);
 
// Add node to end of linked list 
void append(struct LList* list, int value);
 
// Utility function to print a linked list 
void printList(struct LList** list);

//Utility function to generate random linked list of size n
void randomLinkedList(struct LList** list, int n);



//------------------Sorts------------------

// Insertion sort algorithm applied to linked list, returns array 
void insertionSortLL(struct LList** list, int* arr);

//Bucket sort using insertion sort and linkeds list as buckets 
void bucketSortLL(int* arr, int n, int max, int n_buckets);