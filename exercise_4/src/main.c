#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "pthreadBarrier.h"
#include "spinBarrier.h"
#include "condBarrier.h"

int main(int argc, char *argv[])
{
    int thread_count, iterations;

    config(argc, argv, &iterations, &thread_count);

    // printf(">>> Starting mutual_exclution...\n");
    pthread_barrier(thread_count, iterations);
    // printf(">>> Starting atomic_operations...\n");
    condition_barrier(thread_count, iterations);

    spin_barrier(thread_count, iterations);

    return EXIT_SUCCESS;
}
