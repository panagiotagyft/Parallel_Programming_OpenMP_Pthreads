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

    config(argc, argv, &num_darts, &thread_count);

    serial(num_darts);
    open_mp(num_darts, thread_count);
    pthreads_impl(num_darts, thread_count);

    return EXIT_SUCCESS;
}