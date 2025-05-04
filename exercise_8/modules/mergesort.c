#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifdef _OPENMP
#include <omp.h>
#endif

// Function to merge two sorted subarrays into one sorted arra
void merge(int *array, int left, int middle, int right)
{
    int i, j, l, s1, s2;

    // Calculate sizes of the two subarrays
    s1 = middle - left + 1;
    s2 = right - middle;

    // Allocate auxiliary array to hold the merged elements
    int *auxiliary_array = malloc((s1 + s2) * sizeof(int));
    if (auxiliary_array == NULL)
    {
        write(STDERR_FILENO, "ERROR Not available memory\n", 27);
        exit(EXIT_FAILURE);
    }

    // Copy data to the auxiliary array: first left subarray, then right subarray
    for (i = 0; i < s1 + s2; i++)
    {
        if (i < s1)
        {
            auxiliary_array[i] = array[left + i];
            continue;
        }

        auxiliary_array[i] = array[middle + 1 + (i - s1)];
    }

    // Merge the two subarrays back into the original array
    i = 0;
    j = s1;
    l = left;
    while (i < s1 && j < (s1 + s2))
    {
        if (auxiliary_array[i] > auxiliary_array[j])
        {
            array[l++] = auxiliary_array[j++];
            continue;
        }

        array[l++] = auxiliary_array[i++];
    }

    // -- Copy any remaining elements from the left & right subarray --
    while (i < s1)
        array[l++] = auxiliary_array[i++];

    while (j < (s1 + s2))
        array[l++] = auxiliary_array[j++];
    
    // -- 

    free(auxiliary_array);

    return;
}

// Recursive serial merge sort function
void serial_mergeSort(int *array, int left, int right)
{
    if (left == right)
        return;

    int middle = left + ((right - left) >> 1);

    serial_mergeSort(array, left, middle);
    serial_mergeSort(array, middle + 1, right);

    merge(array, left, middle, right);
}

// Recursive parallel merge sort using OpenMP
void parallel_mergeSort(int *array, int left, int right)
{
    if (left == right)
        return;

    int middle = left + ((right - left) >> 1);

    // Create parallel tasks only if the array size is large enough 10^6
#pragma omp task shared(array) if (right - left > 1000000)
    parallel_mergeSort(array, left, middle);

#pragma omp task shared(array) if (right - left > 1000000)
    parallel_mergeSort(array, middle + 1, right);

#pragma omp taskwait
    merge(array, left, middle, right);
}