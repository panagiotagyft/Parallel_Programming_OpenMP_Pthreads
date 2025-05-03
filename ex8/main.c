#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "mergesort.h"

#ifdef _OPENMP
#include <omp.h>
#endif

// Function to print an array
void printArray(int A[], int size)
{
    int i;
    for (i = 0; i < 100000; i++)
        printf("%d ", A[i]);
    printf("\n");
}

bool isnumber(const char *);

int main(int argc, char *argv[])
{
    long long int array_size;
    int parallel_impl, thread_count;
    double total_time, start, end;

    srand(time(NULL));

    if (argc < 4)
    {
        write(STDOUT_FILENO, "Missing arguments...\n", 21);
        return EXIT_FAILURE;
    }
    else if (argc > 4)
    {
        write(STDOUT_FILENO, "Too many arguments...\n", 22);
        return EXIT_FAILURE;
    }
    else
    {
        if (!isnumber(argv[1]) || !isnumber(argv[2]) || !isnumber(argv[3]))
        {
            write(STDOUT_FILENO, "Arguments must be positive integers!!\n", 38);
            return EXIT_FAILURE;
        }

        array_size = atoi(argv[1]);
        parallel_impl = atoi(argv[2]);
        thread_count = atoi(argv[3]);
        printf("%d\n", parallel_impl);
        if (parallel_impl != 0 && parallel_impl != 1)
        {
            write(STDOUT_FILENO, "Second argument must be 0 (serial) or 1 (parallel).\n", 52);
            return EXIT_FAILURE;
        }
    }

    int *array = (int *)malloc(array_size * sizeof(int));
    if (array == NULL)
    {
        write(STDERR_FILENO, "ERROR Not available memory\n", 27);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < array_size; i++)
        array[i] = rand() % array_size;

    // printf("Unsorted array array is \n");
    // printArray(array, array_size);
    if (parallel_impl == 1)
    {
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
        printf("Parallel process takes %f seconds to execute\n", total_time);
    }
    else
    {
        start = ((double)clock()) / CLOCKS_PER_SEC;
        serial_mergeSort(array, 0, array_size - 1);
        end = ((double)clock()) / CLOCKS_PER_SEC;
        total_time = end - start;
        printf("Serial process takes %f seconds to execute\n", total_time);
    }

    printArray(array, array_size);
    return EXIT_SUCCESS;
}

bool isnumber(const char *str)
{
    if (*str == '\0')
        return false;

    do
    {
        if (*str < '0' || *str > '9')
            return false;
        str++;
    } while (*str);

    return true;
}
