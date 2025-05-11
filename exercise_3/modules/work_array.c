#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include "work_array.h"

static pthread_t *worker_threads = NULL;
int *A = NULL;

struct thread_arg
{
    int iterations;
    int thread_id;
};
struct thread_arg *args = NULL;

void handle_sigint_array(void)
{
    printf("\n[!] Caught Ctrl+C (SIGINT). Cleaning up...\n");

    if (worker_threads != NULL)
        free(worker_threads);
    
    if (A != NULL)
        free(A);
        
    if (args != NULL)
        free(args);
}

// Thread function that performs work with mutex-protected counter
void *culc_sum(void *arg)
{
    struct thread_arg *t = arg;
    for (int i = 0; i < t->iterations; i++)
        A[t->thread_id]++;
    
    pthread_exit(NULL);
}

void *culc_sum_opt(void *arg)
{
    struct thread_arg *t = arg;
    long local_count = 0;
    for (int i = 0; i < t->iterations; i++)
        local_count++;
    
    // Μια μόνο εγγραφή στο κοινό array
    A[t->thread_id] = (int)local_count;
    pthread_exit(NULL);
}

void work_array(int thread_count, int iterations, int *array, bool opt)
{
    double total_time, start, end;
    int iters;
    int flag_remainder = true;

    // 1) Allocate memory for A
    A = malloc(thread_count * sizeof(int));
    if (A == NULL)
    {
        write(STDERR_FILENO, "ERROR Not available memory\n", 27);
        exit(EXIT_FAILURE);
    }

    // 2a) Copy with memcpy
    memcpy(A, array, thread_count * sizeof(int));

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

    struct thread_arg *args = malloc(thread_count * sizeof *args);
    for (long thread = 0; thread < thread_count; thread++)
    {
        if (flag_remainder == true)
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
        if (opt == true)
            error = pthread_create(&worker_threads[thread], NULL, culc_sum_opt, &args[thread]);
        else
            error = pthread_create(&worker_threads[thread], NULL, culc_sum, &args[thread]);

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
    const char *label = opt ? "Optimization" : "Without Optimization";
    printf("%s,%d,%d,%.6f\n", label, thread_count, iterations, total_time);
    memcpy(array, A, thread_count * sizeof(int));
    // Free memory and destroy mutex
    free(worker_threads);
    free(args);
}