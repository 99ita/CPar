#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*

Bucket sort: Pega na lista de inteiros com N elementos e divide os elementos em N buckets
             cada bucket corresponde a um intervalo de valores o primeiro vai de 0 a X 
             e o ultimo vai de Y ao maior valor do array. 
             Depois cada bucket é sorted usando outro algoritmo (tipicamente insertion sort
             por se comportar como O(n) quando os elementos nao estao muito longe da posicao final). 
             No final percorre-se cada bucket por ordem e copia-se os valores para o array final

Plano:
    -Percorrer o array para encontrar o valor maximo* ou usar o MAX_RAND?

    -Buckets podem ser:
        -Arrays dinamicos: Chato de implementar e pode ser pouco eficiente pois, como lidamos com arrays 
                           pequenos, temos que estar sempre a copiar o array (assumindo que o array começa
                           com size 1 e dobra sempre que é necessario mais, poderia ser implementada outra 
                           solucao aqui)
        
        -Arrays Estaticos: Consome muita memória (O(n^2) em vez de O(n)) mas muito facil de implementar

        -Listas ligadas: Resolve o problema dos arrays dinamicos e o overhead de memória não e muito grande
                         nesta estrategia ao ordenar os buckets podia-se logo ordenar para um array usando 
                         insertion sort

    -Percorrer o array para preencher os buckets*

    -Percorrer os buckets ordenando-os*

    -Qual a melhor maneira de juntar os arrays ordenados correspondentes a cada bucket?
        -Pode se tentar criar logo o array total e depois percorrer os buckets sequencialmente (dificil de paralelizar)
        -Dividir o sorting dos buckets por threads e atribuir uma tag a cada pacote de buckets (que será assigned a uma thread)
        de forma a mais tarde conseguirmos juntar todos


    *: Zonas com asterisco são as que me parecem ser paralelizáveis
*/







// Linked list structure to represent a bucket
struct Node{
    int data;
    struct Node *next;
};
 
// Add node to head of linked list 
void push(struct Node** head, int value){
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
 
    new_node->data  = value;
 
    new_node->next = (*head);
 
    (*head)    = new_node;
}
 
// Add node after input node 
void insertAfter(struct Node* prev_node, int new_data){
    if (prev_node == NULL){
      printf("ERROR: The given previous node cannot be NULL!");
      return;
    }
 
    struct Node* new_node =(struct Node*) malloc(sizeof(struct Node));
 
    new_node->data  = new_data;
 
    new_node->next = prev_node->next;
 
    prev_node->next = new_node;
}
 
// Add node to end of linked list 
void append(struct Node** head_ref, int new_data){
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
    struct Node *last = *head_ref;  
 
    new_node->data  = new_data;
 
    new_node->next = NULL;
 
    if (*head_ref == NULL){
       *head_ref = new_node;
       return;
    }
 
    while (last->next != NULL)
        last = last->next;
 

    last->next = new_node;
    return;
}
 
// Utility function to print a linked list
void printList(struct Node *node)
{
    while (node != NULL)
    {
       printf(" %d ", node->data);
       node = node->next;
    }
    printf("\n\n");
}

/* Insertion sort algorithm complexity O(n^2) but in cases 
where elements are close to their final position behaves like O(n)
applied to*/
void insertionSortLL(int* arr, int n){
    int i, key, j;

    // Assumes first element is ordered (i = 1)
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
 
        // Shifts elements greater than key one position ahead
        while(j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }

        // Position of current key found
        arr[j + 1] = key;
    }
}


























void bucketSort(int* array, int n){  
    int i, j;  
    int count[n]; 
    for (i = 0; i < n; i++)
        count[i] = 0;
 
    for (i = 0; i < n; i++)
        (count[array[i]])++;
 
    for (i = 0, j = 0; i < n; i++)  
        for(; count[i] > 0; (count[i])--)
            array[j++] = i;
}   




/* Insertion sort algorithm complexity O(n^2) but in cases 
where elements are close to their final position behaves like O(n)*/
void insertionSort(int* arr, int n){
    int i, key, j;

    // Assumes first element is ordered (i = 1)
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
 
        // Shifts elements greater than key one position ahead
        while(j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }

        // Position of current key found
        arr[j + 1] = key;
    }
}
 
//Utility function to print the array
void printArr(int* arr, int n){
    int i;
    for (i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

//Utility function to generate random array of size n
int* randArr(int n){
   int* randArr = (int*) malloc(sizeof(int)*n);
   for(int i = 0; i < n; i++) randArr[i]=rand();   //Generate number between 0 to 99
   return randArr;
}

//Function to validate argument
int valid(char* s){
    return atoi(s);
}

 
//Driver
int main(int argc, char* argv[]){
    int size;


    struct Node* head = NULL;
 
    append(&head, 6);
 
    push(&head, 7);
 
    push(&head, 1);
 
    append(&head, 4);
 
  
    insertAfter(head->next, 8);
 
    printf("\n Created Linked list is: ");
    printList(head);
 
    return 0;







/*
    //----------Input control----------
    if(argc < 2){
        printf("\nERROR: Not enough arguments!\n./seq <Size of Array>\n\n");
        return -1;
    }
    if(argc > 2){
        printf("\nERROR: Too many arguments!\n./seq <Size of Array>\n\n");
        return -1;
    }
    if(!(size = valid(argv[1]))){
        printf("\nERROR: Argument invalid!\n./seq <Size of Array>\n\n");
        return -1;
    }
    //-------------------------------------


    printf("\nRAND_MAX: %d\nSize of Array: %d\n\n",RAND_MAX,size);

    int* arr = randArr(size);
 
    clock_t begin = clock();
    insertionSort(arr, size);
    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\nTime elapsed: %fs\n\n",time_spent);

    clock_t begin2 = clock();
    bucketSort(arr, size);
    clock_t end2 = clock();

    double time_spent2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;

    printf("\nTime elapsed: %fs\n\n",time_spent2);*/

}