#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include "linkedList.h"
#include "arrays.h"


#include "papi.h"

// PAPI counters
#define NUM_EVENTS 4
int Events[NUM_EVENTS] = {PAPI_TOT_CYC, PAPI_TOT_INS, PAPI_L1_TCM, PAPI_L2_TCM};
long long values[NUM_EVENTS];
int retval, EventSet=PAPI_NULL;

//Executes splits times the chosen algorithm on an array of size n;
//sort == 0 - quicksort; 
//sort == 1 - insertionsort; 
//sort == 2 - bucketsort using insertionsort;
//sort == 3 - bucketsort using quicksort;
void sorts(int sort, int n, int splits, int max, int n_buckets, int n_threads){
    if(n_buckets == 0) n_buckets = floor((double)n/64);
    if(n_threads == 0) n_threads = 4;
    if(sort==0) printf("\nQuickSort (%d elements) (%d splits)...",n,splits);
    if(sort==1) printf("\nInsertionSort (%d elements) (%d splits)...",n,splits);
    if(sort==2) printf("\nBucketSort (InsertionSort) (%d buckets) (%d elements) (%d splits)...",n_buckets,n,splits);
    if(sort==3) printf("\nBucketSort (QuickSort) (%d buckets) (%d elements) (%d splits)...",n_buckets,n,splits);
    if(sort==4) printf("\nBucketSort Pararellized %d threads (Shared buckets: InsertionSort) (%d buckets) (%d elements) (%d splits)...",n_threads,n_buckets,n,splits);
    if(sort==5) printf("\nBucketSort Pararellized %d threads (Shared buckets: QuickSort) (%d buckets) (%d elements) (%d splits) ...",n_threads,n_buckets,n,splits);
    if(sort==6) printf("\nBucketSort on Linked List (InsertionSort) (%d buckets) (%d elements) (%d splits)...",n_buckets,n,splits);
    if(sort==7) printf("\nBucketSort on DynamicArray (InsertionSort) (%d buckets) (%d elements) (%d splits)...",n_buckets,n,splits);
    if(sort==8) printf("\nBucketSort on DynamicArray (QuickSort) (%d buckets) (%d elements) (%d splits) ...",n_buckets,n,splits);
    if(sort==9) printf("\nBucketSort Pararellized %d threads (Thread owned: InsertionSort) (%d buckets) (%d elements) (%d splits)...",n_threads,n_buckets,n,splits);
    if(sort==10) printf("\nBucketSort Pararellized %d threads (Thread owned: QuickSort) (%d buckets) (%d elements) (%d splits) ...",n_threads,n_buckets,n,splits);
    double tmin = 0, tmax = 0;
    double timeB = omp_get_wtime();
    int* arr;


    /* Start counting events */
    if (PAPI_start(EventSet) != PAPI_OK) {
        fprintf (stderr, "PAPI error starting counters!\n");
        return;
    }
    for(int i = 0; i < splits; i++){
        srand(1);
        arr = randomArr(n);

        double begin = omp_get_wtime();
        if(sort==0) quickSort(arr, n);
        if(sort==1) insertionSort(arr, n);
        if(sort==2) arr = bucketSort(arr, n, max, n_buckets, 0);
        if(sort==3) arr = bucketSort(arr, n, max, n_buckets, 1);
        if(sort==4) arr = bucketSortPar(arr, n, max, n_buckets, 0, n_threads);
        if(sort==5) arr = bucketSortPar(arr, n, max, n_buckets, 1, n_threads);
        if(sort==6) bucketSortLL(arr, n, max, n_buckets);
        if(sort==7) bucketSortDynArr(arr, n, max, n_buckets, 0);
        if(sort==8) bucketSortDynArr(arr, n, max, n_buckets, 1);
        if(sort==9) arr = bucketSortParSplit(arr, n, max, n_buckets, 0, n_threads);
        if(sort==10) arr = bucketSortParSplit(arr, n, max, n_buckets, 1, n_threads);
        double end = omp_get_wtime();;
        
        double time_spent = (double)(end - begin);
        if(i == 0){
            tmin = time_spent;
            tmax = time_spent;
        }else{
            if(time_spent > tmax) tmax = time_spent;
            if(time_spent < tmin) tmin = time_spent;
        }
        //verifyOrdered(arr, n);
        free(arr);
    }
    double timeE = omp_get_wtime();
    //Stop counting events
    if(PAPI_stop(EventSet,values) != PAPI_OK) {
        fprintf (stderr, "PAPI error stoping counters!\n");
        return;
    }

    double total_time = timeE - timeB;
    printf("\n\nTotal: %.4fs\nAverage: %.4fs\nMin: %fs\nMax: %fs\n\n",total_time, total_time/(double)splits, tmin, tmax);
}

//Parses an int from input
int parseInt(char* arg){
    double d;
    int r = sscanf(arg, "%lf", &d);
    if(r == EOF) return -1;
    else return floor(d);
}

//Parses first argument of input
int parseArg(char* arg){
    if(arg[0] != '-') return 0;
    if(arg[1] == 's') return 1;
    if(arg[1] == 'b'){
        if(arg[2] == 's') return 2;
        if(arg[2] == 'p'){
            if(arg[3] == '1') return 3;
            else if(arg[3] == '2') return 6;
        } 
        if(arg[2] == 'l') return 4;
        if(arg[2] == 'd') return 5;
    }
    if(arg[1] == 'a') return 7;
    return 0;
}

//Deals with the input arguments
int inputControl(int argc, char* argv[], int* mode, int* size, int* splits, int* n_buckets, int* n_threads){
    //----------Input control----------
    int r = 1;
    if(argc == 1){
        r = 0;
    }
    else if(!(*mode = parseArg(argv[1]))){
        r = 0;
    }
    else if(argc == 3){
        r = 0;
    }
    else if(*mode == 1){
        if(!(*size = parseInt(argv[2])) || !(*splits = parseInt(argv[3]))){
            r = 0;
        }
    }else if(*mode >= 2 && *mode < 6 && *mode != 3){
        if((*n_buckets = parseInt(argv[2])) == -1 || !(*size = parseInt(argv[3])) || !(*splits = parseInt(argv[4]))){
            r = 0;
        }
    }else if(*mode == 3 || *mode == 6){
        if(!(*n_threads = parseInt(argv[2])) || (*n_buckets = parseInt(argv[3])) == -1 || !(*size = parseInt(argv[4])) || !(*splits = parseInt(argv[5]))){
            r = 0;
        }
    }
    if(r == 0){
        printf("\nUsage:\n./run [-bs|bd|bl n_buckets] [-bp1|bp2 n_threads n_buckets] [-s] arraysize splits\n");
        printf("-s compares insertion sort and quick sort\n");
        printf("-bl sequential implementation of bucket sort with linked lists as buckets\n");
        printf("-bs compares sequential implementations of bucket sort using insertion sort or quick sort with arrays as buckets\n");
        printf("-bp1 compares parallel implementations of bucket sort using insertion sort or quick sort with arrays as buckets (global buckets)\n");
        printf("-bp2 compares parallel implementations of bucket sort using insertion sort or quick sort with arrays as buckets (thread owned buckets)\n");
        printf("-bd compares sequencial implementations of bucket sort using insertion sort or quick sort with dynamic arrays as buckets\n\n");
        printf("-a final testing\n\n");
    }
    return r;
}


void runall(){
    int L1Size = 8192;
    int L2Size = 65536;
    int L3Size = 2097152;
    int RamSize = 4194304;

    printf("Initializing all tests\n\n\n\n");

    printf("L1 Cache test\n");
    //L1
    sorts(2,L1Size,1,RAND_MAX,0,8); //Sequential using insertion sort
    sorts(3,L1Size,1,RAND_MAX,0,8); //Sequential using quick sort
    sorts(4,L1Size,1,RAND_MAX,0,8); //Pararellized1 using insertion sort
    sorts(5,L1Size,1,RAND_MAX,0,8); //Pararellized1 using quick sort
    sorts(9,L1Size,1,RAND_MAX,0,8); //Pararellized2 using insertion sort
    sorts(10,L1Size,1,RAND_MAX,0,8); //Pararellized2 using quick sort


    printf("L2 Cache test\n");
    //L2
    sorts(2,L2Size,1,RAND_MAX,0,8); //Sequential using insertion sort
    sorts(3,L2Size,1,RAND_MAX,0,8); //Sequential using quick sort
    sorts(4,L2Size,1,RAND_MAX,0,8); //Pararellized1 using insertion sort
    sorts(5,L2Size,1,RAND_MAX,0,8); //Pararellized1 using quick sort
    sorts(9,L2Size,1,RAND_MAX,0,8); //Pararellized2 using insertion sort
    sorts(10,L2Size,1,RAND_MAX,0,8); //Pararellized2 using quick sort


    printf("L3 Cache test\n");
    //L3
    sorts(2,L3Size,1,RAND_MAX,0,8); //Sequential using insertion sort
    sorts(3,L3Size,1,RAND_MAX,0,8); //Sequential using quick sort
    sorts(4,L3Size,1,RAND_MAX,0,8); //Pararellized1 using insertion sort
    sorts(5,L3Size,1,RAND_MAX,0,8); //Pararellized1 using quick sort
    sorts(9,L3Size,1,RAND_MAX,0,8); //Pararellized2 using insertion sort
    sorts(10,L3Size,1,RAND_MAX,0,8); //Pararellized2 using quick sort


    printf("Ram test\n");
    //RAM
    sorts(2,RamSize,1,RAND_MAX,0,8); //Sequential using insertion sort
    sorts(3,RamSize,1,RAND_MAX,0,8); //Sequential using quick sort
    sorts(4,RamSize,1,RAND_MAX,0,8); //Pararellized1 using insertion sort
    sorts(5,RamSize,1,RAND_MAX,0,8); //Pararellized1 using quick sort
    sorts(9,RamSize,1,RAND_MAX,0,8); //Pararellized2 using insertion sort
    sorts(10,RamSize,1,RAND_MAX,0,8); //Pararellized2 using quick sort
}



//Driver
int main(int argc, char* argv[]){
    srand(1);
    int mode, size, splits, n_buckets, n_threads, num_hwcntrs = 0;

    fprintf (stdout, "\nSetting up PAPI...");
    // Initialize PAPI
    retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
        fprintf(stderr,"PAPI library init error!\n");
        return 0;
    }
    
    /* create event set */
    if (PAPI_create_eventset(&EventSet) != PAPI_OK) {
        fprintf(stderr,"PAPI create event set error\n");
        return 0;
    }
    
    /* Get the number of hardware counters available */
    if ((num_hwcntrs = PAPI_num_hwctrs()) <= PAPI_OK)  {
        printf("\nn: %d\n",num_hwcntrs);
        fprintf (stderr, "PAPI error getting number of available hardware counters!\n");
        return 0;
    }
    fprintf(stdout, "done!\nThis system has %d available counters.\n\n", num_hwcntrs);
    
    // We will be using at most NUM_EVENTS counters
    if (num_hwcntrs >= NUM_EVENTS) {
        num_hwcntrs = NUM_EVENTS;
    } else {
        fprintf (stderr, "Error: there aren't enough counters to monitor %d events!\n", NUM_EVENTS);
        return 0;
    }
    
    if (PAPI_add_events(EventSet,Events,NUM_EVENTS) != PAPI_OK)  {
        fprintf(stderr,"PAPI library add events error!\n");
        return 0;
    }


    if(inputControl(argc, argv, &mode, &size, &splits, &n_buckets,&n_threads) == 0) return 0;

    if(mode == 1){ //Array sorts
        sorts(0,size,splits,RAND_MAX,n_buckets, n_threads);
        sorts(1,size,splits,RAND_MAX,n_buckets, n_threads);
    }else if(mode == 2){ //Bucket sort sequential arrays
        sorts(2,size,splits,RAND_MAX,n_buckets, n_threads);
        sorts(3,size,splits,RAND_MAX,n_buckets, n_threads);
    }else if(mode == 3){ //Bucket sort parallel arrays
        sorts(4,size,splits,RAND_MAX,n_buckets, n_threads);
        sorts(5,size,splits,RAND_MAX,n_buckets, n_threads);
    }else if(mode == 4){ //Bucket sort sequential linked list
        sorts(6,size,splits,RAND_MAX,n_buckets, n_threads);
    }else if(mode == 5){ //Bucket sort sequential dynamic arrays
        sorts(7,size,splits,RAND_MAX,n_buckets, n_threads);
        sorts(8,size,splits,RAND_MAX,n_buckets, n_threads);
    }else if(mode == 6){ //Bucket sort parallel arrays (thread owned)
        sorts(9,size,splits,RAND_MAX,n_buckets, n_threads);
        sorts(10,size,splits,RAND_MAX,n_buckets, n_threads);
    }else if(mode == 7){
        runall();
    }


    // output PAPI counters' values
    for (int i=0 ; i< NUM_EVENTS ; i++){
        char EventCodeStr[PAPI_MAX_STR_LEN];
        
        if (PAPI_event_code_to_name(Events[i], EventCodeStr) == PAPI_OK) {
            fprintf (stdout, "%s = %lld\n", EventCodeStr, values[i]);
        } else {
            fprintf (stdout, "PAPI UNKNOWN EVENT = %lld\n", values[i]);
        }
    }

    return 0;
}
