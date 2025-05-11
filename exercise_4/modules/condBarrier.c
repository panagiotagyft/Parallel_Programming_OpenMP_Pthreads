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

/* Clean‐up on Ctrl+C */
void handle_sigint_barrierCond(int sig)
{
    (void)sig;
    fprintf(stderr, "\n[!] Caught SIGINT, cleaning up…\n");
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
        pthread_mutex_lock(&barrier_mutex);

        barrier_thread_cnt++;
        if (barrier_thread_cnt == thread_count)
        {
            barrier_thread_cnt = 0;
            pthread_cond_broadcast(&ok_to_proceed);
        }
        else
        {
            while (pthread_cond_wait(&ok_to_proceed, &barrier_mutex) != 0)
                ; /* retry if spuriously awakened */
        }

        pthread_mutex_unlock(&barrier_mutex);
    }
    pthread_exit(NULL);
}

void condition_barrier(int threads, int num_iterations)
{
    thread_count = threads;
    iterations = num_iterations;

    signal(SIGINT, handle_sigint_barrierCond);

    worker_threads_c = malloc(thread_count * sizeof(pthread_t));
    if (!worker_threads_c)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    double start = (double)clock() / CLOCKS_PER_SEC;
    for (long t = 0; t < thread_count; t++)
    {
        int err = pthread_create(&worker_threads_c[t],
                                 NULL,
                                 test_barrier_cond,
                                 (void *)t);
        if (err)
        {
            fprintf(stderr, "pthread_create: %s\n", strerror(err));
            exit(EXIT_FAILURE);
        }
    }

    for (long t = 0; t < thread_count; t++)
    {
        pthread_join(worker_threads_c[t], NULL);
    }
    double end = (double)clock() / CLOCKS_PER_SEC;

    printf("Barrier Condition Variables, %ld, %ld,%.6f\n",thread_count, iterations, end - start);

    free(worker_threads_c);
    pthread_mutex_destroy(&barrier_mutex);
    pthread_cond_destroy(&ok_to_proceed);
}