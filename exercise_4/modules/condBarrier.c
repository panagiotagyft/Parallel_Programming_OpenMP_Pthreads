#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include "condBarrier.h"

/* Shared state for the barrier */
long thread_count = 0;
long iterations = 0;
long barrier_thread_cnt = 0;
pthread_mutex_t barrier_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ok_to_proceed = PTHREAD_COND_INITIALIZER;
pthread_t *worker_threads_c = NULL;

// handle_sigint_barrierCond: Cleanup resources on Ctrl+C (SIGINT)
void handle_sigint_barrierCond(int sig)
{
    (void)sig;
    fprintf(stderr, "\n[!] Caught SIGINT, cleaning up…\n");

    // Free allocated thread array, destroy sync primitives
    if (worker_threads_c)
        free(worker_threads_c);
    pthread_mutex_destroy(&barrier_mutex);
    pthread_cond_destroy(&ok_to_proceed);

    exit(EXIT_FAILURE);
}

void *test_barrier_cond(void *rank)
{
    long my_rank = (long)rank;
    (void)my_rank;

    for (long i = 0; i < iterations; i++)
    {
        // Lock mutex before accessing shared counter
        pthread_mutex_lock(&barrier_mutex);

        barrier_thread_cnt++;
        if (barrier_thread_cnt == thread_count)
        {
            // Last thread to reach the barrier resets the counter and wakes up all waiting threads.
            barrier_thread_cnt = 0;
            pthread_cond_broadcast(&ok_to_proceed);
        }
        else
        {
            // Other threads wait on the condition variable until they are signaled by the last thread.
            // The loop handles spurious wakeups.
            while (pthread_cond_wait(&ok_to_proceed, &barrier_mutex) != 0); /* retry if spuriously awakened */
        }

        // Unlock mutex to allow next barrier phase
        pthread_mutex_unlock(&barrier_mutex);
    }

    pthread_exit(NULL);
}

void condition_barrier(int threads, int num_iterations)
{
    double total_time;
    struct timespec start, end;

    // Initialize global state
    thread_count = threads;
    iterations = num_iterations;

    signal(SIGINT, handle_sigint_barrierCond);

    // Allocate array for thread handles
    worker_threads_c = malloc(thread_count * sizeof(pthread_t));
    if (!worker_threads_c)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    clock_gettime(CLOCK_MONOTONIC, &start);


    for (long t = 0; t < thread_count; t++)
    {
        int error = pthread_create(&worker_threads_c[t],NULL,test_barrier_cond,(void *)t);
        if (error != 0)
        {
            fprintf(stderr, "ERROR: pthread_create() failed: %s\n", strerror(error));
            exit(EXIT_FAILURE);
        }
    }

    for (long t = 0; t < thread_count; t++)
    {
        pthread_join(worker_threads_c[t], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    total_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Barrier Condition Variables, %ld, %ld,%.6f\n", thread_count, iterations, total_time);

    free(worker_threads_c);
    pthread_mutex_destroy(&barrier_mutex);
    pthread_cond_destroy(&ok_to_proceed);
}