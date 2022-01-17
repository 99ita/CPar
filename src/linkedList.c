#include "linkedList.h"

//------------------Utilities------------------
 
// Add node to head of linked list 
void push(struct LList** list, int value){
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
    
    if((*list) == NULL){
        (*list) = (struct LList*) malloc(sizeof(struct LList));
        (*list)->size = 0;
    }
    newNode->value  = value;

    newNode->next = (*list)->head;
 
    (*list)->head = newNode;

    (*list)->size++;
}
 
// Add node to end of linked list 
void append(struct LList* list, int value){
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
    struct Node* last = list->head;  
 
    newNode->value  = value;
 
    newNode->next = NULL;
 
    if(list->head == NULL){
        list->head = newNode;
        list->size++;
        return;
    }
 
    while (last->next != NULL)
        last = last->next;
 

    last->next = newNode;
    list->size++;
    return;
}
 
// Utility function to print a linked list 
void printList(struct LList** list){
    printf("dadad \n\n");
    struct Node* node = (*list)->head;
    printf("dadad \n\n");
    int empty = 1;
    while(node != NULL){
        empty = 0;
        printf(" %d ", node->value);
        node = node->next;
    }
    if(empty) printf("Empty List");
    printf("\n\n");
}

//Utility function to generate random linked list of size n
void randomLinkedList(struct LList** list, int n){
    if((*list)->head != NULL) return; //Head must be NULL
    for(;n > 0; n--){
        push(list,rand());
    }
}



//------------------Sorts------------------

// Insertion sort algorithm applied to linked list, returns array 
void insertionSortLL(struct LList** list, int* arr){
    struct Node* node = (*list)->head;

    for(int i = 0; node != NULL; i++){
        int key = node->value;
        if(i == 0){ //if first element insert to first position of array
            arr[0] = key;
        }else{ //else insert orderly
            int j = i-1;
            while(j >= 0 && arr[j] > key){ //While the value is greater than key shift the value forward by one 
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key; ///When the value stops being greater than key we found key's position
        }
        node = node->next;
    }
}   

//Bucket sort using insertion sort and linkeds list as buckets 
void bucketSortLL(int* arr, int n, int max, int n_buckets){
    
    struct LList** bucketArr = (struct LList**) calloc(n_buckets,sizeof(struct LList*));
    
    int bucketId;
    for(int i = 0; i < n; i++){
        //printf("\n1i: %d",i);
        bucketId = floor((double) n_buckets*arr[i]/max); //Assigns all the values into one of n_buckets buckets 
        if(bucketId>n_buckets-1) bucketId = n_buckets-1;
        //printf("\n2i: %d,%d",i,bucketId);
        push(&(bucketArr[bucketId]),arr[i]);
    }

    int arrI = 0;
    for(int i = 0; i < n_buckets; i++){ //Hard to parallelize
        if(bucketArr[i] != NULL){
            insertionSortLL(&bucketArr[i],arr+arrI);
            arrI += bucketArr[i]->size;
        }
    }   
}