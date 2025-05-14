#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include "work_array.h"

// Global pointer for the integer array used by threads
int *array = NULL;

int main(int argc, char *argv[])
{
    int thread_count, iterations;

    // Parse command-line arguments and initialize 'iterations' and 'thread_count'
    config(argc, argv, &iterations, &thread_count);

    // Allocate an integer array of size 'thread_count'
    array = calloc(thread_count, sizeof(int));
    if (array == NULL)
    {
        // Print an error message if memory allocation fails
        write(STDERR_FILENO, "ERROR Not available memory\n", 27);
        return EXIT_FAILURE;
    }

    // Perform the work without optimization
    work_array(thread_count, iterations, array, false);

    // Sum up results from each thread
    int sum = 0;
    for (int i = 0; i < thread_count; i++)
    {
        sum += array[i];
    }
    // printf("Sum: %d\n", sum);

    // Free the allocated memory
    free(array);

    // Allocate the array again for the mutual exclusion test
    array = calloc(thread_count, sizeof(int));
    if (array == NULL)
    {
        write(STDERR_FILENO, "ERROR Not available memory\n", 27);
        return EXIT_FAILURE;
    }

    // Perform the work with optimization
    work_array(thread_count, iterations, array, true);

    // Reset and compute the sum of results after locking
    sum = 0;
    for (int i = 0; i < thread_count; i++)
    {
        sum += array[i];
    }
    // printf("Sum: %d\n", sum);

    // Clean up memory before exiting
    free(array);

    return EXIT_SUCCESS;
}
