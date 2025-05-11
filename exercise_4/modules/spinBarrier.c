#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include "spinBarrier.h"


int thread_count;
int iterations;
int barrier_count = 0;
pthread_mutex_t barrier_mutex_s = PTHREAD_MUTEX_INITIALIZER;
int flag = 0;

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
        pthread_mutex_lock(&barrier_mutex_s);
        barrier_count++;
        if (barrier_count == thread_count)
        {
            pthread_mutex_unlock(&barrier_mutex_s);
            barrier_count = 0;
            flag = local_sense; // release waiting threads
        }
        else
        {
            pthread_mutex_unlock(&barrier_mutex_s);
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
    double total_time, start, end;
    thread_count = threads;
    iterations = num_of_iterations;

    signal(SIGINT, handle_sigint_barrierSence);

    // Allocate memory for thread handles
    if ((worker_threads_s = malloc(thread_count * sizeof(pthread_t))) == NULL)
    {
        perror("ERROR: malloc() failed");
        exit(EXIT_FAILURE);
    }

    start = ((double)clock()) / CLOCKS_PER_SEC;

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
    end = ((double)clock()) / CLOCKS_PER_SEC;

    total_time = end - start;
    printf("Centralized Barrier with Sense Reversal,%d,%d,%.6f\n", thread_count, iterations, total_time);

    // Free memory and destroy barrier
    pthread_mutex_destroy(&barrier_mutex_s);
    free(worker_threads_s);
}