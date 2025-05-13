#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h> // for intptr_t
#include "atomic_operations.h"

static int shared_var = 0;
static pthread_t *worker_threads = NULL;

void handle_sigint_atomic()
{
    printf("\n[!] Caught Ctrl+C (SIGINT). Cleaning up...\n");

    if (worker_threads != NULL)
        free(worker_threads);
}

// Worker thread function: performs atomic increments
void *culc_sum_atomic(void *arg)
{
    intptr_t iters = (intptr_t)arg;
    for (intptr_t i = 0; i < iters; i++)
    {
        // Atomically increment shared_var with sequential consistency
        __atomic_fetch_add(&shared_var, 1, __ATOMIC_SEQ_CST);
    }
    pthread_exit(NULL);
}

void atomic_operations(int thread_count, int iterations, int var)
{

    double total_time, start, end;
    intptr_t iters;

    // Register signal handler for cleanup
    signal(SIGINT, handle_sigint_atomic);

    // Initialize the shared variable to the given starting value
    shared_var = var;

    // Determine if there is a remainder when dividing work among threads
    int flag_remainder = true;
    int remainder = iterations % thread_count;
    if (remainder == 0)
    {
        flag_remainder = false;
    }

    // Allocate memory for thread handles
    if ((worker_threads = malloc(thread_count * sizeof(pthread_t))) == NULL)
    {
        perror("ERROR: malloc() failed");
        exit(EXIT_FAILURE);
    }

    start = ((double)clock()) / CLOCKS_PER_SEC;

    // Create threads
    for (long thread = 0; thread < thread_count; thread++)
    {
        if (flag_remainder == true)
        {
            iters = (intptr_t)(iterations / thread_count) + remainder;
            flag_remainder = false;
        }
        else
        {
            iters = (intptr_t)(iterations / thread_count);
        }
        int error = pthread_create(&worker_threads[thread], NULL, culc_sum_atomic, (void *)iters);
        if (error != 0)
        {
            fprintf(stderr, "ERROR: pthread_create() failed: %s\n", strerror(error));
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < thread_count; i++)
    {
        pthread_join(worker_threads[i], NULL);
    }

    end = ((double)clock()) / CLOCKS_PER_SEC;

    total_time = end - start;
    printf("Atomic,%d,%d,%.6f\n", thread_count, iterations, total_time);

    // Free memory and destroy mutex
    free(worker_threads);
}