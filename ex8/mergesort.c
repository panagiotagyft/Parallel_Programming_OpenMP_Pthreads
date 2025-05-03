#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifdef _OPENMP
#include <omp.h>
#endif

void merge(int *array, int left, int middle, int right)
{
    int i, j, l, s1, s2;

    s1 = middle - left + 1;
    s2 = right - middle;

    int *auxiliary_array = malloc((s1 + s2) * sizeof(int));
    if (auxiliary_array == NULL)
    {
        write(STDERR_FILENO, "ERROR Not available memory\n", 27);
        exit(EXIT_FAILURE);
    }

    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < s1 + s2; i++)
    {
        if (i < s1)
        {
            auxiliary_array[i] = array[left + i];
            continue;
        }

        auxiliary_array[i] = array[middle + 1 + (i - s1)];
    }

    // Merge the temp arrays back into arr[l..r
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

    // Copy the remaining elements of L[],
    // if there are any
    while (i < s1)
        array[l++] = auxiliary_array[i++];

    while (j < (s1 + s2))
        array[l++] = auxiliary_array[j++];

    free(auxiliary_array);

    return;
}


void serial_mergeSort(int *array, int left, int right)
{
    if (left == right)
        return;

    int middle = left + ((right - left) >> 1);

    serial_mergeSort(array, left, middle);
    serial_mergeSort(array, middle + 1, right);

    merge(array, left, middle, right);
}

// l is for left index and r is right index of the
// sub-array of arr to be sorted
void parallel_mergeSort(int *array, int left, int right)
{
    if (left == right)
        return;

    int middle = left + ((right - left) >> 1);

#pragma omp task shared(array) if (right - left > 1000000)
    parallel_mergeSort(array, left, middle);

#pragma omp task shared(array) if (right - left > 1000000)
    parallel_mergeSort(array, middle + 1, right);

#pragma omp taskwait
    merge(array, left, middle, right);
}