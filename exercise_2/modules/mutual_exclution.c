#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include "mutual_exclution.h"

// Global mutex for protecting access to shared_var
pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER;
int shared_var;
static pthread_t *worker_threads = NULL;

void handle_sigint_mutex(int sig)
{
    (void)sig;
    printf("\n[!] Caught Ctrl+C (SIGINT). Cleaning up...\n");

    if (worker_threads != NULL)
        free(worker_threads);

    pthread_mutex_destroy(&Mutex);
}

// Thread function that performs work with mutex-protected counter
void *culc_sum(void *arg)
{
    intptr_t iters = (intptr_t)arg;
    for (intptr_t i = 0; i < iters; i++)
    {
        if (pthread_mutex_lock(&Mutex) != 0)
        {
            (void)write(STDOUT_FILENO, "ERROR: pthread_mutex_unlock() failed\n", 37);
            exit(EXIT_FAILURE);
        }
        // Critical section: increment the shared counter
        shared_var++;
        if (pthread_mutex_unlock(&Mutex) != 0)
        {
            (void)write(STDOUT_FILENO, "ERROR: pthread_mutex_unlock() failed\n", 37);
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit(NULL);
}

int mutual_exclution(int thread_count, int iterations, int var)
{
    double total_time;
    struct timespec start, end;
    intptr_t iters;
    shared_var = var;
    int flag_remainder = true;

    // Register signal handler for cleanup
    signal(SIGINT, handle_sigint_mutex);

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

    clock_gettime(CLOCK_MONOTONIC, &start);

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
        int error = pthread_create(&worker_threads[thread], NULL, culc_sum, (void *)iters);
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
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    total_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Mutex,%d,%d,%.6f\n", thread_count, iterations, total_time);

    // Free memory and destroy mutex
    free(worker_threads);
    pthread_mutex_destroy(&Mutex);

    return shared_var;
}