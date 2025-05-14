#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include "work_array.h"

// Array of worker thread IDs
static pthread_t *worker_threads = NULL;
// Shared array for thread results
int *A = NULL;

// Arguments passed to each thread
struct thread_arg
{
    int iterations; // Number of loop iterations this thread should perform
    int thread_id;  // Index into the shared array A
};
struct thread_arg *args = NULL;

// Signal handler to catch Ctrl+C (SIGINT) and free allocated memory
void handle_sigint_array(int sig)
{
    (void)sig; // Unused parameter
    printf("\n[!] Caught Ctrl+C (SIGINT). Cleaning up...\n");

    if (worker_threads != NULL)
        free(worker_threads);
    if (A != NULL)
        free(A);
    if (args != NULL)
        free(args);

    exit(EXIT_FAILURE);
}

// Thread function without optimization: increments shared array element each iteration
void *culc_sum(void *arg)
{
    struct thread_arg *t = arg;
    for (int i = 0; i < t->iterations; i++)
    {
        // Increment the shared counter for this thread
        A[t->thread_id]++;
    }
    pthread_exit(NULL);
}

// Thread function with optimization: uses a local counter and writes back once
void *culc_sum_opt(void *arg)
{
    struct thread_arg *t = arg;
    long local_count = 0;
    for (int i = 0; i < t->iterations; i++)
    {
        // Bump only the local counter
        local_count++;
    }
    // One single write to the shared array
    A[t->thread_id] = (int)local_count;
    pthread_exit(NULL);
}

// Main function to distribute work across threads and measure execution time
void work_array(int thread_count, int iterations, int *array, bool opt)
{
    double total_time;
    struct timespec start, end;
    int iters;
    bool flag_remainder = true;

    // Install signal handler for graceful cleanup
    signal(SIGINT, handle_sigint_array);

    // 1) Allocate memory for the shared array A
    A = malloc(thread_count * sizeof(int));
    if (A == NULL)
    {
        write(STDERR_FILENO, "ERROR Not available memory\n", 27);
        exit(EXIT_FAILURE);
    }

    // 2) Copy initial values from caller’s array into A
    memcpy(A, array, thread_count * sizeof(int));

    // Determine if there’s a remainder when dividing work evenly
    int remainder = iterations % thread_count;
    if (remainder == 0)
    {
        flag_remainder = false;
    }

    // Allocate array of pthread_t handles
    worker_threads = malloc(thread_count * sizeof(pthread_t));
    if (worker_threads == NULL)
    {
        perror("ERROR: malloc() failed");
        exit(EXIT_FAILURE);
    }

    clock_gettime(CLOCK_MONOTONIC, &start);

    // Allocate argument structs for each thread
    args = malloc(thread_count * sizeof *args);
    if (args == NULL)
    {
        perror("ERROR: malloc() failed for args");
        exit(EXIT_FAILURE);
    }

    // Launch threads
    for (int thread = 0; thread < thread_count; thread++)
    {
        // Distribute the extra iterations to the first thread if needed
        if (flag_remainder)
        {
            iters = (iterations / thread_count) + remainder;
            flag_remainder = false;
        }
        else
        {
            iters = iterations / thread_count;
        }

        args[thread].iterations = iters;
        args[thread].thread_id = thread;

        int error;
        if (opt)
        {
            // Use optimized function
            error = pthread_create(&worker_threads[thread], NULL,
                                   culc_sum_opt, &args[thread]);
        }
        else
        {
            // Use non-optimized function
            error = pthread_create(&worker_threads[thread], NULL,
                                   culc_sum, &args[thread]);
        }

        if (error != 0)
        {
            fprintf(stderr,
                    "ERROR: pthread_create() failed: %s\n",
                    strerror(error));
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < thread_count; i++)
    {
        pthread_join(worker_threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    total_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Print a CSV line: label, thread count, total iterations, elapsed time
    const char *label = opt ? "Optimization" : "Without Optimization";
    printf("%s,%d,%d,%.6f\n", label, thread_count, iterations, total_time);

    // Copy results back into caller’s array
    memcpy(array, A, thread_count * sizeof(int));

    // Clean up
    free(worker_threads);
    free(args);
    free(A);
}
