#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include "mutual_exclution.h"


int thread_count, iterations;
pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER;
long shared_var = 0;
static pthread_t *worker_threads = NULL;

void handle_sigint_mutex(void)
{
    printf("\n[!] Caught Ctrl+C (SIGINT). Cleaning up...\n");

    if (worker_threads != NULL)
        free(worker_threads);

    pthread_mutex_destroy(&Mutex);
}

// Thread function that performs work with mutex-protected counter
void *culc_sum(void *rank)
{
    (void)rank;
    // long my_rank = (long)rank;
    // printf("Thread %ld is running (Threads=%d).\n", my_rank+1, thread_count);

    for (int i = 0; i < iterations; i++)
    {
        if (pthread_mutex_lock(&Mutex) != 0)
        {
            (void)write(STDOUT_FILENO, "ERROR: pthread_mutex_unlock() failed\n", 37);
            exit(EXIT_FAILURE);
        }
        shared_var++;
        if (pthread_mutex_unlock(&Mutex) != 0)
        {
            (void)write(STDOUT_FILENO, "ERROR: pthread_mutex_unlock() failed\n", 37);
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit(NULL);
}

void mutual_exclution(int threads, int num_of_iterations)
{
    double total_time, start, end;

    thread_count = threads;
    iterations = num_of_iterations;

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
        int error = pthread_create(&worker_threads[thread], NULL, culc_sum, (void *)thread);
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
    printf("Mutex,%d,%d,%.6f\n", thread_count, iterations, total_time);

    // Free memory and destroy mutex
    free(worker_threads);
    pthread_mutex_destroy(&Mutex);

}