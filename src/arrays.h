#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include "minheap.h"

//------------------Utilities------------------
typedef struct {
  int *array;
  size_t used;
  size_t size;
} DynArray;

void initDynArray(DynArray *a, size_t initialSize);

void insertDynArray(DynArray *a, int element);

void freeDynArray(DynArray *a);

//Set minimum and maximum of array
void minmax(double* arr, int n, double* min, double* max);

//Calculates mean of array
double mean(double* arr, int n);

// Verifies if an array is ordered
void verifyOrdered(int* arr, int n);

//Compares two integeres
int cmpfunc (const void * a, const void * b);

//Utility function to print the array 
void printArr(int* arr, int n);

//Utility function to generate random array of size n 
int* randomArr(int n);


//------------------Sorts------------------

// Quick Sort Algorithm complexity O(nlogn)
void quickSort(int* arr, int n);

/* Insertion sort algorithm complexity O(n^2) but in cases 
where elements are close to their final position may have 
better performance than quick sort*/ 
void insertionSort(int* arr, int n);

/*Bucket sort implemented with both quicksort and insertionsort. 
When input is uniformly distributed has an expected complexity of O(n)*/
int* bucketSort(int* arr, int n, int max, int n_buckets, int sort);

int* bucketSortPar(int* arr, int n, int max, int n_buckets, int sort, int n_threads);

int* bucketSortParSplit(int* arr, int n, int max, int n_buckets, int sort, int n_threads);

void bucketSortDynArr(int* arr, int n, int max, int n_buckets, int sort);
 

 