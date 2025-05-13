#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include "ex1_Pthreads.h"

pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex to protect shared variable `arrows`
long long int arrows = 0;
static pthread_t *worker_threads = NULL; // Array to hold thread handles

// Structure to pass arguments to each thread
typedef struct thread_args
{
    long long int num_darts; // Number of random points (darts) to throw
    unsigned int seed;       // Seed for rand_r()
} thread_args_t;

// Signal handler for Ctrl+C (SIGINT) to clean up resources
void handle_sigint_mutex(int sig)
{
    (void)sig;
    printf("\n[!] Caught Ctrl+C (SIGINT). Cleaning up...\n");

    // Free the thread handles array if allocated
    if (worker_threads != NULL)
        free(worker_threads);

    // Destroy the mutex
    pthread_mutex_destroy(&Mutex);
    exit(EXIT_FAILURE);
}

// Main function to run the Monte Carlo simulation with pthreads
int pthreads_impl(long long int num_darts, int thread_count)
{
    double total_time;
    struct timespec start, end;
    long long int iters;
    int flag_remainder = true;
    thread_args_t *args;

    // Register signal handler for cleanup
    signal(SIGINT, handle_sigint_mutex);

    // Determine if there is a remainder when dividing darts among threads
    int remainder = num_darts % thread_count;
    if (remainder == 0)
        flag_remainder = false;
    

    // Allocate memory for thread handles
    if ((worker_threads = malloc(thread_count * sizeof(pthread_t))) == NULL)
    {
        perror("ERROR: malloc() failed");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for thread arguments
    if((args = malloc(thread_count * sizeof *args)) == NULL)
    {
        perror("ERROR: malloc() failed");
        exit(EXIT_FAILURE);
    }


    clock_gettime(CLOCK_MONOTONIC, &start);

    // Create each worker thread and assign its portion of darts
    for (long thread = 0; thread < thread_count; thread++)
    {
        if (flag_remainder == true)
        {
            iters = (num_darts / thread_count) + remainder;
            flag_remainder = false;
        }
        else
        {
            iters = num_darts / thread_count;
        }
        
        args[thread].num_darts = iters;
        args[thread].seed = (unsigned int)time(NULL) ^ thread;

        int error = pthread_create(&worker_threads[thread], NULL, thread_func, &args[thread]);
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
    printf("Pthreads,%d,%lld,%lld,%.6f\n", thread_count, num_darts, arrows, total_time);

    // Free memory and destroy mutex
    free(worker_threads);
    free(args);
    pthread_mutex_destroy(&Mutex);

    return EXIT_SUCCESS;
}

// Function executed by each worker thread
void *thread_func(void *arg)
{
    double x, y, squared_dist;
    long long int count_in_circle = 0;
    struct thread_args *args = arg;

    // Generate `num_darts` random points and count how many land inside the unit circle
    for (long long int throw = 0; throw < args->num_darts; throw++)
    {

        x = (double)rand_r(&args->seed) / RAND_MAX * 2.0 - 1.0; // float in range -1 to 1
        y = (double)rand_r(&args->seed) / RAND_MAX * 2.0 - 1.0;

        squared_dist = (x * x) + (y * y);

        if (squared_dist <= 1)
            count_in_circle++;
    }

    // Safely add this thread's count to the global `arrows` variable
    if (pthread_mutex_lock(&Mutex) != 0)
    {
        (void)write(STDOUT_FILENO, "ERROR: pthread_mutex_unlock() failed\n", 37);
        exit(EXIT_FAILURE);
    }
    arrows += count_in_circle;
    if (pthread_mutex_unlock(&Mutex) != 0)
    {
        (void)write(STDOUT_FILENO, "ERROR: pthread_mutex_unlock() failed\n", 37);
        exit(EXIT_FAILURE);
    }

    pthread_exit(NULL);
}
