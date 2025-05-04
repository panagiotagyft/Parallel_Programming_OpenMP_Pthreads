#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "mergesort.h"
#include "config.h"

#ifdef _OPENMP
#include <omp.h>
#endif

// // Function to print an array
// void printArray(int A[], int size)
// {
//     int i;
//     for (i = 0; i < 100000; i++)
//         printf("%d ", A[i]);
//     printf("\n");
// }


int main(int argc, char *argv[])
{
    long long int array_size;
    int parallel_impl, thread_count;
    double total_time, start, end;

    srand(10);

    // Read configuration from command-line arguments
    config(argc, argv, &array_size, &parallel_impl, &thread_count);

    // Allocate memory for the array
    int *array = (int *)malloc(array_size * sizeof(int));
    if (array == NULL)
    {
        write(STDERR_FILENO, "ERROR Not available memory\n", 27);
        return EXIT_FAILURE;
    }

    // Fill the array with random values
    for (int i = 0; i < array_size; i++)
        array[i] = rand() % array_size;

    if (parallel_impl == 1)
    { // Run parallel merge sort
        start = omp_get_wtime();
        omp_set_num_threads(thread_count);
#pragma omp parallel
        {
#pragma omp single
            {
                parallel_mergeSort(array, 0, array_size - 1);
            }
        }
        end = omp_get_wtime();
        total_time = end - start;
        printf("%lld,%d,%f\n", array_size, thread_count, total_time);
        // printf("Parallel process takes %f seconds to execute\n", total_time);
    }
    else
    { // Run serial merge sort
        start = ((double)clock()) / CLOCKS_PER_SEC;
        serial_mergeSort(array, 0, array_size - 1);
        end = ((double)clock()) / CLOCKS_PER_SEC;
        total_time = end - start;

        printf("%lld,serial,%f\n", array_size, total_time);
        // printf("Serial process takes %f seconds to execute\n", total_time);
    }

    return EXIT_SUCCESS;
}