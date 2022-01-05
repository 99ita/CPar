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
                         insertion sort (é preciso saber o tamanho total do bucket:
                                            -criar uma "superstruct" que guarda sempre o tamanho da ll
                                            -percorrer a ll para saber o tamanho do bucket)

        Ja implementei insertionsort para arrays e para listas ligadas a ordenar logo para um array

    -Percorrer o array para preencher os buckets*

    -Percorrer os buckets ordenando-os*

    -Qual a melhor maneira de juntar os arrays ordenados correspondentes a cada bucket?*
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
 
// Add node to head of linked list (net)
void push(struct Node** head, int value){
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
 
    newNode->data  = value;
 
    newNode->next = (*head);
 
    (*head)    = newNode;
}
 
// Add node after input node (net)
void insertAfter(struct Node* prevNode, int value){
    if (prevNode == NULL){
      printf("ERROR: The given previous node cannot be NULL!");
      return;
    }
 
    struct Node* newNode =(struct Node*) malloc(sizeof(struct Node));
 
    newNode->data  = value;
 
    newNode->next = prevNode->next;
 
    prevNode->next = newNode;
}
 
// Add node to end of linked list (net)
void append(struct Node** head, int value){
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
    struct Node *last = *head;  
 
    newNode->data  = value;
 
    newNode->next = NULL;
 
    if (*head == NULL){
       *head = newNode;
       return;
    }
 
    while (last->next != NULL)
        last = last->next;
 

    last->next = newNode;
    return;
}
 
// Utility function to print a linked list (net)
void printList(struct Node *node){
    while (node != NULL){
        printf(" %d ", node->data);
        node = node->next;
    }
    printf("\n\n");
}

// Insertion sort algorithm applied to linked list, returns array (eu)
int* insertionSortLL(struct Node** head, int size){
    struct Node* node = *head;
    int* arr = (int*) malloc(sizeof(int)*size);

    for(int i = 0; node != NULL; i++){
        int key = node->data;
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
    return arr;
}   

//Utility function to generate random linked list of size n(eu)
void randomLinkedList(struct Node** head, int n){
    if(*head!=NULL) return; //Head must be NULL
    srand(getpid());
    for(;n > 0; n--){
        push(head,rand());
    }
}
























void bucketSort(int* array, int n){ //(net, nao funciona se o valor mais alto for maior que o numero de valores)
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

void verifyOrdered(int* arr, int size){
    for(int i = 1; i < size; i++){
        if(arr[i-1] > arr[i]){
            printf("Array isn't ordered!\nArray[%d] = %d and Array[%d] = %d",i-1,i,arr[i-1],arr[i]);
            return;
        }
    }
    printf("Array is ordered!\n");
}


/* Insertion sort algorithm complexity O(n^2) but in cases 
where elements are close to their final position behaves like O(n)*/ //(net)
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
 
//Utility function to print the array (net)
void printArr(int* arr, int n){
    int i;
    for (i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

//Utility function to generate random array of size n (eu)
int* randomArr(int n){
    srand(getpid());
    int* randArr = (int*) malloc(sizeof(int)*n);
    for(int i = 0; i < n; i++) randArr[i]=rand();   //Generate number between 0 to 99
    return randArr;
}



//Function to validate argument (eu)
int valid(char* s){
    return atoi(s);
}

 
//Driver
int main(int argc, char* argv[]){
    int size;

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



    printf("\nRAND_MAX: %d\nSize of Array: %d\n",RAND_MAX,size);

    int* arr = randomArr(size);
 
    clock_t begin = clock();
    insertionSort(arr, size);
    verifyOrdered(arr, size);
    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\nArray to Array\nTime elapsed: %fs\n",time_spent);




    struct Node* head = NULL;
    randomLinkedList(&head,size);

    clock_t begin2 = clock();
    verifyOrdered(insertionSortLL(&head, size),size);
    clock_t end2 = clock();

    double time_spent2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;

    printf("\nLinked list to Array\nTime elapsed: %fs\n\n",time_spent2);
    
    return 0;
}