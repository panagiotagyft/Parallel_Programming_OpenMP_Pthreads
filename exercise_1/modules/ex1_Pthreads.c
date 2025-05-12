#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include "ex1_Pthreads.h"

pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER;
long long int arrows = 0;
static pthread_t *worker_threads = NULL;

void handle_sigint_mutex(int sig)
{
    (void)sig;
    printf("\n[!] Caught Ctrl+C (SIGINT). Cleaning up...\n");

    if (worker_threads != NULL)
        free(worker_threads);

    pthread_mutex_destroy(&Mutex);
}

void *thread_func(void *arg)
{
    double x, y, squared_dist;

    intptr_t iters = (intptr_t)arg;
    for (intptr_t i = 0; i < iters; i++)
    {
        unsigned int seed = (unsigned int)time(NULL) ^ i; // απλός seed
        x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;     // float in range -1 to 1
        y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        // printf("x: %f, y: %f\n", x, y);

        if (pthread_mutex_lock(&Mutex) != 0)
        {
            (void)write(STDOUT_FILENO, "ERROR: pthread_mutex_unlock() failed\n", 37);
            exit(EXIT_FAILURE);
        }

        squared_dist = (x * x) + (y * y);

        if (squared_dist <= 1)
            arrows++;

        if (pthread_mutex_unlock(&Mutex) != 0)
        {
            (void)write(STDOUT_FILENO, "ERROR: pthread_mutex_unlock() failed\n", 37);
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit(NULL);
}

int pthreads_impl(long long int num_darts, int thread_count)
{
    double total_time, start, end;
    intptr_t iters;
    int flag_remainder = true;

    signal(SIGINT, handle_sigint_mutex);

    int remainder = num_darts % thread_count;
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
            iters = (intptr_t)(num_darts / thread_count) + remainder;
            flag_remainder = false;
        }
        else
        {
            iters = (intptr_t)(num_darts / thread_count);
        }
        int error = pthread_create(&worker_threads[thread], NULL, thread_func, (void *)iters);
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
    printf("Pthreads,%d,%lld,%lld,%.6f\n", thread_count, num_darts, arrows, total_time);

    // Free memory and destroy mutex
    free(worker_threads);
    pthread_mutex_destroy(&Mutex);

    return EXIT_SUCCESS;
}
