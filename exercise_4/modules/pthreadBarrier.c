#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include "pthreadBarrier.h"


int thread_count;
int iterations;
pthread_barrier_t barrier;
pthread_t *worker_threads = NULL;


void handle_sigint_barrierPthread(int sig)
{
    (void)sig;
    printf("\n[!] Caught Ctrl+C (SIGINT). Cleaning up...\n");

    if (worker_threads != NULL)
        free(worker_threads);
   
    pthread_barrier_destroy(&barrier);
}

void *test_barrier_pthread(void *rank)
{
    (void)rank;
    for (long i = 0; i < iterations; i++)
    {
        pthread_barrier_wait(&barrier);
    }
    pthread_exit(NULL);
}

void pthread_barrier(int threads, int num_of_iterations)
{
    double total_time, start, end;
    thread_count = threads;
    iterations = num_of_iterations;

    signal(SIGINT, handle_sigint_barrierPthread);

    // Allocate memory for thread handles
    if ((worker_threads = malloc(thread_count * sizeof(pthread_t))) == NULL)
    {
        perror("ERROR: malloc() failed");
        exit(EXIT_FAILURE);
    }

    if ((pthread_barrier_init(&barrier, NULL, thread_count)) != 0)
    {
        perror("Error initializing barrier\n");
        exit(EXIT_FAILURE);
    }

    start = ((double)clock()) / CLOCKS_PER_SEC;

    // Create threads
    for (long thread = 0; thread < thread_count; thread++)
    {
        int error = pthread_create(&worker_threads[thread], NULL, test_barrier_pthread, (void *)thread);
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
    printf("Pthread Barrier,%d,%d,%.6f\n", thread_count, iterations, total_time);

    // Free memory and destroy barrier
    pthread_barrier_destroy(&barrier);
    free(worker_threads);
}