#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include "spinBarrier.h"


int thread_count, iterations;
int barrier_count = 0, flag = 0;
pthread_mutex_t barrier_mutex_s = PTHREAD_MUTEX_INITIALIZER;


pthread_t *worker_threads_s = NULL;

void handle_sigint_barrierSence(int sig)
{
    (void)sig;
    printf("\n[!] Caught Ctrl+C (SIGINT). Cleaning up...\n");

    if (worker_threads_s != NULL)
        free(worker_threads_s);
    
    pthread_mutex_destroy(&barrier_mutex_s);
}

void *test_sense_reversal_centralized_barrier(void *rank)
{
    (void)rank;
    bool local_sense = false;
    for (long i = 0; i < iterations; i++)
    {
        local_sense = !local_sense; // toggle sense

        // Enter critical section to update barrier_count
        pthread_mutex_lock(&barrier_mutex_s);
        barrier_count++;

        // If this is the last thread to enter barrier
        if (barrier_count == thread_count)
        {
            pthread_mutex_unlock(&barrier_mutex_s); // unlock mutex before resetting
            barrier_count = 0;  // reset counter for next iteration
            flag = local_sense; // release waiting threads
        }
        else
        {
            pthread_mutex_unlock(&barrier_mutex_s); // unlock mutex and wait for flag update
            while (flag != local_sense)
            {
                ; // busy wait
            }
        }
    }
    pthread_exit(NULL);
}

void spin_barrier(int threads, int num_of_iterations)
{
    double total_time;
    struct timespec start, end;
    thread_count = threads;
    iterations = num_of_iterations;

    signal(SIGINT, handle_sigint_barrierSence);

    // Allocate memory for thread handles
    if ((worker_threads_s = malloc(thread_count * sizeof(pthread_t))) == NULL)
    {
        perror("ERROR: malloc() failed");
        exit(EXIT_FAILURE);
    }

    clock_gettime(CLOCK_MONOTONIC, &start);

    // Create threads
    for (long thread = 0; thread < thread_count; thread++)
    {
        int error = pthread_create(&worker_threads_s[thread], NULL, test_sense_reversal_centralized_barrier, (void *)thread);
        if (error != 0)
        {
            fprintf(stderr, "ERROR: pthread_create() failed: %s\n", strerror(error));
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < thread_count; i++)
    {
        pthread_join(worker_threads_s[i], NULL);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    total_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("Centralized Barrier with Sense Reversal,%d,%d,%.6f\n", thread_count, iterations, total_time);

    // Free memory and destroy barrier
    pthread_mutex_destroy(&barrier_mutex_s);
    free(worker_threads_s);
}