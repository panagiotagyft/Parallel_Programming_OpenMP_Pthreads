#ifndef MERGESORT_H
#define MERGESORT_H

// Function to merge two sorted subarrays into one sorted arra
void merge(int *, int, int, int);

// Recursive parallel merge sort using OpenMP
void parallel_mergeSort(int *, int, int);

// Recursive serial merge sort function
void serial_mergeSort(int *, int, int);

#endif // MERGESORT_H