#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "linkedList.h"
#include "arrays.h"

//Executes splits times the chosen algorithm on an array of size n;
//sort == 0 - quicksort; 
//sort == 1 - insertionsort; 
//sort == 2 - bucketsort using insertionsort;
//sort == 3 - bucketsort using quicksort;
void sorts(int sort, int n, int splits, int max, int n_buckets){
    if(sort==0) printf("\nQuickSort (%d elements) (%d splits)...",n,splits);
    if(sort==1) printf("\nInsertionSort (%d elements) (%d splits)...",n,splits);
    if(sort==2) printf("\nBucketSort (InsertionSort) (%d elements) (%d splits) (%d buckets)...",n,splits,n_buckets);
    if(sort==3) printf("\nBucketSort (QuickSort) (%d elements) (%d splits) (%d buckets)...",n,splits,n_buckets);
    double tmin = 0, tmax = 0;
    clock_t begin_tot = clock();
    int* arr;
    for(int i = 0; i < splits; i++){
        srand(time(NULL));
        arr = randomArr(n);
    
        clock_t begin = clock();
        if(sort==0) quickSort(arr, n);
        if(sort==1) insertionSort(arr, n);
        if(sort==2) arr = bucketSort(arr, n, max, n_buckets, 0);
        if(sort==3) arr = bucketSort(arr, n, max, n_buckets, 1);
        clock_t end = clock();
        
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        if(i == 0){
            tmin = time_spent;
            tmax = time_spent;
        }else{
            if(time_spent > tmax) tmax = time_spent;
            if(time_spent < tmin) tmin = time_spent;
        }
        verifyOrdered(arr, n);
        if(splits < 20) printf("\nSplit %d/%d",i+1,splits);
        //free(arr);
    }
    clock_t end_tot = clock();

    double total_time = (double)(end_tot - begin_tot) / CLOCKS_PER_SEC;
    printf("\n\nTotal: %.4fs\nAverage: %.4fs\nMin: %fs\nMax: %fs\n\n",total_time, total_time/(double)splits, tmin, tmax);
}

//Parses an int from input
int parseInt(char* arg){
    double d;
    int r = sscanf(arg, "%lf", &d);
    if(r == EOF || r == 0) return 0;
    else return floor(d);
}

//Parses first argument of input
int parseArg(char* arg){
    if(arg[0] != '-') return 0;
    if(arg[1] == 's') return 1;
    if(arg[1] == 'b') return 2;
    return 0;
}

//Deals with the input arguments
int inputControl(int argc, char* argv[], int* mode, int* size, int* splits, int* n_buckets){
    //----------Input control----------
    if(argc == 1){
        printf("\nUsage:\n./run [-s] [-b n_buckets] arraysize splits\n");
        printf("-s compares insertion sort and quick sort\n-b compares bucket sorts using insertion sort or quick sort\n\n");
        return 0;
    }
    if(!(*mode = parseArg(argv[1]))){
        printf("\nERROR!\n./run [-s] [-b n_buckets] arraysize splits\n");
        printf("-s compares insertion sort and quick sort\n-b compares bucket sorts using insertion sort or quick sort\n\n");
           return 0;
    }
    if(argc == 3){
        printf("\nERROR!\n./run [-s] [-b n_buckets] arraysize splits\n");
        printf("-s compares insertion sort and quick sort\n-b compares bucket sorts using insertion sort or quick sort\n\n");
        return 0;
    }
    else if(*mode == 1){
        if(!(*size = parseInt(argv[2])) || !(*splits = parseInt(argv[3]))){
            printf("\nERROR!\n./run [-s] [-b n_buckets] arraysize splits\n");
            printf("-s compares insertion sort and quick sort\n-b compares bucket sorts using insertion sort or quick sort\n\n");
            return 0;
        }
    }else if(*mode == 2){
        if(!(*n_buckets = parseInt(argv[2])) || !(*size = parseInt(argv[3])) || !(*splits = parseInt(argv[4]))){
            printf("\nERROR!\n./run [-s] [-b n_buckets] arraysize splits\n");
            printf("-s compares insertion sort and quick sort\n-b compares bucket sorts using insertion sort or quick sort\n\n");
            return 0;
        }
    }
    return 1;
}


//Driver
int main(int argc, char* argv[]){
    srand(getpid());
    int mode, size, splits, n_buckets;

    if(inputControl(argc, argv, &mode, &size, &splits, &n_buckets) == 0) return 0;

    if(mode == 1){ //Array sorts
        sorts(0,size,splits,RAND_MAX,n_buckets);
        sorts(1,size,splits,RAND_MAX,n_buckets);

    }else if(mode == 2){ //Bucket sort
        sorts(2,size,splits,RAND_MAX,n_buckets);
        sorts(3,size,splits,RAND_MAX,n_buckets);        
    }
    return 0;
}


