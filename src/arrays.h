#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Set minimum and maximum of array
void minmax(double* arr, int n, double* min, double* max);

//Calculates mean of array
double mean(double* arr, int count);

// Verifies if an array is ordered
void verifyOrdered(int* arr, int n);

/* Insertion sort algorithm complexity O(n^2) but in cases 
where elements are close to their final position behaves like O(n)*/
void insertionSort(int* arr, int n);

/* Insertion sort algorithm complexity O(n^2) but in cases 
where elements are close to their final position behaves like O(n)*/
void quickSort(int* arr, int n);
 
//Utility function to print the array
void printArr(int* arr, int n);

//Utility function to generate random array of size n
int* randomArr(int n);

int cmpfunc (const void * a, const void * b);