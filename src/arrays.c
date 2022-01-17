#include "arrays.h"

//------------------Utilities------------------

void initArray(DynArray *a, size_t initialSize) {
  a->array = malloc(initialSize * sizeof(int));
  a->used = 0;
  a->size = initialSize;
}

void insertArray(DynArray *a, int element) {
  // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
  // Therefore a->used can go up to a->size 
  if (a->used == a->size) {
    a->size = (a->size*3)/2+8;
    a->array = realloc(a->array, a->size * sizeof(int));
  }
  a->array[a->used++] = element;
}

void freeArray(DynArray *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

//Set minimum and maximum of array
void minmax(double* arr, int n, double* min, double* max){
    *min = arr[0];
    *max = arr[0];
    for (int i = 1; i < n; i++){
        if(arr[i] > *max) *max = arr[i];
        if(arr[i] < *min) *min = arr[i];
    }
}

//Calculates mean of array
double mean(double* arr, int n){
    double sum = arr[0];
    for(int i = 1; i < n; i++){
        sum += arr[i];
    }
    return sum/n;
}

// Verifies if an array is ordered
void verifyOrdered(int* arr, int n){
    for(int i = 1; i < n; i++){
        if(arr[i-1] > arr[i]){
            printf("Array isn't ordered!\n...\n[%d] = %d \n[%d] = %d\n...\n",i-1,arr[i-1],i,arr[i]);
            return;
        }
    }
}

//Compares two integeres
int cmpfunc (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

//Utility function to print the array 
void printArr(int* arr, int n){
    int i;
    printf("\n");
    for (i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

//Utility function to generate random array of size n 
int* randomArr(int n){
    int* randArr = (int*) calloc(n,sizeof(int));
    for(int i = 0; i < n; i++) randArr[i]=rand();   //Generate number between 0 to RAND_MAX
    return randArr;
}




//------------------Sorts------------------

// Quick Sort Algorithm complexity O(nlogn)
void quickSort(int* arr, int n){
    qsort(arr, n, sizeof(int), cmpfunc);
}

/* Insertion sort algorithm complexity O(n^2) but in cases 
where elements are close to their final position may have 
better performance than quick sort*/ 
void insertionSort(int* arr, int n){ 
    int i, key, j;

    // Assumes first element is ordered (i = 1)
    for(i = 1; i < n; i++) {
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

/* Insertion sort algorithm complexity O(n^2) but in cases 
where elements are close to their final position may have 
better performance than quick sort*/ 
void insertionSortOut(int* bucket, int size, int* arr, int start){ 
    int i, key, j;
    // Assumes first element is ordered (i = 1)
    arr[start] = bucket[0];
    for(i = 1; i < size; i++){
        key = bucket[i];
        j = i - 1;
        // Shifts elements greater than key one position ahead
        while(j >= 0 && bucket[j] > key){
            arr[start + j + 1] = arr[start + j];
            j = j - 1;
        }
        // Position of current key found
        arr[start + j + 1] = key;
    }
}

/*Bucket sort implemented with both quicksort and insertionsort. 
When input is uniformly distributed has an expected complexity of O(n)*/
int* bucketSort(int* arr, int n, int max, int n_buckets, int sort){
    //int width = max/n_buckets; //Width of each bucket
	int* buckets = (int*) calloc(n,sizeof(int)); //Array to contain the buckets
	int* size = (int*) calloc(n_buckets,sizeof(int)); //Array to contain the size of each bucket
	int* index = (int*) calloc(n_buckets,sizeof(int)); //Array to contain the index in bucket of next position
	int* start = (int*) calloc(n_buckets,sizeof(int)); //Array to contain the of start position of each bucket

    for(int i = 0; i < n; i++){ //Get size of each bucket
        int bucketId = floor((double) n_buckets*arr[i]/max);
		if (bucketId>n_buckets-1) bucketId = n_buckets-1; //Case for when max isn't divisible by n_buckets
		size[bucketId]++;
    }

    index[0] = 0;
    start[0] = 0;
    for (int i = 1; i < n_buckets; i++){ //Fill the buckets info arrays
		index[i] = index[i-1] + size[i-1];
		start[i] = start[i-1] + size[i-1];
    }

	for(int i = 0; i < n; i++){ //Fill the buckets array
		int bucketId = floor((double) n_buckets*arr[i]/max);
		if(bucketId > n_buckets -1) bucketId = n_buckets-1;
		buckets[index[bucketId]] = arr[i];        
        index[bucketId]++;
	}

    for(int i = 0; i < n_buckets; i++){ //Sort each bucket 
        if(sort == 0) insertionSort(buckets + start[i],size[i]);
        else qsort(buckets + start[i], size[i], sizeof(int), cmpfunc);
    }
    free(arr);
    free(size);
    free(index);
    free(start);
	return buckets;
}




/*Bucket sort implemented with both quicksort and insertionsort. 
When input is uniformly distributed has an expected complexity of O(n)*/
int* bucketSortPar(int* arr, int n, int max, int n_buckets, int sort, int n_threads){
    
    //int width = max/n_buckets; //Width of each bucket
    int* buckets = (int*) calloc(n,sizeof(int)); //Array to contain the buckets
    int* size = (int*) calloc(n_buckets,sizeof(int)); //Array to contain the size of each bucket
    int* index = (int*) calloc(n_buckets,sizeof(int)); //Array to contain the index in bucket of next position
    int* start = (int*) calloc(n_buckets,sizeof(int)); //Array to contain the of start position of each bucket
    //omp_lock_t* locks = (omp_lock_t*) calloc(n_buckets,sizeof(omp_lock_t)); //Array to contain the of start position of each bucket
    int i1,i2,i3,bucketId,bucketId2,b_index;
    
    //int ii,mx = arr[0],mn = arr[0];


    omp_set_num_threads(n_threads);
    /*#pragma omp parallel for private(ii) reduction(max:mx) reduction(min:mn)
    for(ii = 1; ii < n; ii++){
        if(arr[ii] > mx) mx = arr[ii];
        if(arr[ii] < mn) mn = arr[ii];
    }
    max = mx;*/

    //#pragma omp parallel
    //{
        
        //omp_set_num_threads(8);
        /*#pragma omp parallel for private(i)
        for(i = 0; i < n_buckets; i++){
            omp_init_lock(&(locks[i]));
        }*/

        //#pragma omp parallel for private(i1,bucketId) // Each iteration has its own index and calculated id so they need to be private  
        for(i1 = 0; i1 < n; i1++){ //Get size of each bucket
            //printf("i1: %d\nthread: %d\n",i1,myId);
            int val = arr[i1];//-mn;
            bucketId = floor((double) n_buckets*val/max);
            if (bucketId>n_buckets-1) bucketId = n_buckets-1; //Case for when max isn't divisible by n_buckets
        
            //omp_set_lock(&(locks[bucketId]));
            size[bucketId]++;
            //omp_unset_lock(&(locks[bucketId]));
        }
        //printArr(size,n_buckets);
    //}
        index[0] = 0;
        start[0] = 0;
        for (int i = 1; i < n_buckets; i++){ //Fill the buckets info arrays
            index[i] = index[i-1] + size[i-1];
            start[i] = start[i-1] + size[i-1];
        }


        //#pragma omp for private(i2,bucketId2,b_index)
        for(i2 = 0; i2 < n; i2++){ //Fill the buckets array
            int val = arr[i2];//-mn;
            bucketId2 = floor((double) n_buckets*val/max);
            if(bucketId2 > n_buckets -1) bucketId2 = n_buckets-1;
            //omp_set_lock(&(locks[bucketId2]));
            b_index = index[bucketId2];
            buckets[b_index] = arr[i2];        
            index[bucketId2]++;
            //omp_unset_lock(&(locks[bucketId2]));
        }
    //#pragma omp parallel
    //{
        #pragma omp parallel
        { 
            #pragma omp for private(i3)
            for(i3 = 0; i3 < n_buckets; i3++){ //Sort each bucket 
                //printf("i3: %d\nthread: %d\n",i3,myId);
                if(sort == 0) insertionSort(buckets + start[i3],size[i3]);
                else qsort(buckets + start[i3], size[i3], sizeof(int), cmpfunc);
            }
        }

    free(arr);
    free(size);
    free(index);
    free(start);
    return buckets;
}
 





/*Bucket sort implemented with both quicksort and insertionsort. 
When input is uniformly distributed has an expected complexity of O(n)*/
int* bucketSortParSplit(int* arr, int n, int max, int n_buckets, int sort, int n_threads){
    //int width = max/n_buckets; //Width of each bucket
    int** buckets = (int**) calloc(n_threads,sizeof(int*)); //Array to contain the buckets
    int** size = (int**) calloc(n_threads,sizeof(int*)); //Array to contain the size of each bucket
    int** index = (int**) calloc(n_threads,sizeof(int*)); //Array to contain the index in bucket of next position
    int** start = (int**) calloc(n_threads,sizeof(int*)); //Array to contain the of start position of each bucket
    int* sizeThreadBucket = (int*) calloc(n_threads,sizeof(int)); //Array to contain the size of the final sorted array in each thread
    int i1,i2,bucketId,bucketId2,b_index;

    omp_set_num_threads(n_threads);
    #pragma omp parallel
    {
        int myId = omp_get_thread_num();
        buckets[myId] = (int*) calloc(ceil((double)n/n_threads),sizeof(int));
        size[myId] = (int*) calloc(n_buckets,sizeof(int));
        index[myId] = (int*) calloc(n_buckets,sizeof(int));
        start[myId] = (int*) calloc(n_buckets,sizeof(int));
        
        
        #pragma omp for private(i1,bucketId) // Each iteration has its own index and calculated id so they need to be private  
        for(i1 = 0; i1 < n; i1++){ //Get size of each bucket
            bucketId = floor((double) n_buckets*arr[i1]/max);
            if (bucketId>n_buckets-1) bucketId = n_buckets-1; //Case for when max isn't divisible by n_buckets
            sizeThreadBucket[myId]++;
            size[myId][bucketId]++;
        }
        
        
        for (int i = 0; i < n_buckets; i++){ //Fill the buckets info arrays
            if(i == 0){
                index[myId][0] = 0;
                start[myId][0] = 0;
            }else{
                index[myId][i] = index[myId][i-1] + size[myId][i-1];
                start[myId][i] = start[myId][i-1] + size[myId][i-1];
            }
        }
        
        #pragma omp for private(i2,bucketId2,b_index)
        for(i2 = 0; i2 < n; i2++){ //Fill the buckets array
            bucketId2 = floor((double) n_buckets*arr[i2]/max);
            if(bucketId2 > n_buckets -1) bucketId2 = n_buckets-1;
            b_index = index[myId][bucketId2];
            buckets[myId][b_index] = arr[i2];        
            index[myId][bucketId2]++;
        }
        
        int i3;
        for(i3 = 0; i3 < n_buckets; i3++){ //Sort each bucket 
            if(sort == 0) insertionSort(buckets[myId] + start[myId][i3],size[myId][i3]);
            else qsort(buckets[myId] + start[myId][i3], size[myId][i3], sizeof(int), cmpfunc);
        }
        
    }

    
    /*for(int i = 0; i < n_threads; i++){
        printArr(buckets[i],sizeThreadBucket[i]);
    }*/
   
    int* ret = (int*) malloc(sizeof(int)*n);
    minHeap mh = initMinHeap(n_threads);
    for(int i = 0; i < n_threads; i++){
        pushHeap(&mh,buckets[i][0],i);
    }
    
    
    int* currIndBucket = (int*) calloc(sizeof(int),n_threads);
    int ind = 0;
    while(mh.size > 0){
        int thread;
        ret[ind] = popHeap(&mh,&thread);
        currIndBucket[thread]++;
        ind++;
        if(currIndBucket[thread] < sizeThreadBucket[thread]){
            pushHeap(&mh,buckets[thread][currIndBucket[thread]],thread);
        }
        /*printf("\n\n\n");
        printf("currIndBucket[%d]: %d;sizeThreadBucket[%d]: %d; ind: %d\n",thread,currIndBucket[thread],thread,sizeThreadBucket[thread],ind);
        //printArr(buckets[thread]+ind,sizeThreadBucket[thread]-ind);
        printHeap(&mh);
        //printArr(ret,ind);
        printf("\n\n\n");*/
    }

    free(currIndBucket);
    free(sizeThreadBucket);
    for(int i = 0; i < n_threads; i++){
        free(buckets[i]);
        free(size[i]);
        free(index[i]);
        free(start[i]);
    }       
    free(buckets);
    free(size);
    free(index);
    free(start);

    return ret;
}
 


/*Bucket sort implemented with both quicksort and insertionsort. 
When input is uniformly distributed has an expected complexity of O(n)*/
void bucketSortDynArr(int* arr, int n, int max, int n_buckets, int sort){
	DynArray** buckets = (DynArray**) calloc(n_buckets,sizeof(DynArray*));
	for(int i = 0; i < n; i++){ //Fill the buckets array
		int bucketId = floor((double) n_buckets*arr[i]/max);
		if(bucketId > n_buckets -1) bucketId = n_buckets-1;
		if(buckets[bucketId] == NULL){
            buckets[bucketId] = calloc(1,sizeof(DynArray));
            initArray(buckets[bucketId],64);
        }
        insertArray(buckets[bucketId],arr[i]);
	}

    for(int i = 0; i < n_buckets; i++){ //Sort each bucket 
        if(buckets[i]!=NULL){
            if(sort == 0) insertionSort(buckets[i]->array,buckets[i]->used);
            else qsort(buckets[i]->array, buckets[i]->used, sizeof(int), cmpfunc);
        }
    }
    
    int ind = 0;
    for(int i = 0; i < n_buckets; i++){
        if(buckets[i] != NULL){
            for (int j = 0; j < buckets[i]->used; j++){
                arr[ind] = buckets[i]->array[j];
                ind++;
            }
        }
    }
}