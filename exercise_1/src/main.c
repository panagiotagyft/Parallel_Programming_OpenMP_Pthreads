#include <stdio.h>
#include <stdlib.h>
#include "ex1_Pthreads.h"
#include "ex1_Serial.h"
#include "ex1_OpenMP.h"
#include "config.h"

int main(int argc, char *argv[])
{
    long long int num_darts;
    int thread_count;

    srand(10);

    // Parse command-line arguments and set num_darts and thread_count accordingly
    config(argc, argv, &num_darts, &thread_count);

    if (thread_count == 1)
    {
        serial(num_darts); // If only one thread is requested, run the serial implementation
    }
    else
    {
        // Otherwise, run the parallel implementations in two ways:
        // 1) Using OpenMP
        open_mp(num_darts, thread_count);

        // 2) Using Pthreads
        pthreads_impl(num_darts, thread_count);
    }
    return EXIT_SUCCESS;
}