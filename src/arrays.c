#include "arrays.h"

//------------------Utilities------------------

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
            printf("Array isn't ordered!\nArray[%d] = %d and Array[%d] = %d\n",i-1,arr[i-1],i,arr[i]);
            return;
        }
    }
}

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
		int b_index = index[bucketId];
		buckets[b_index] = arr[i];        
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
 

 