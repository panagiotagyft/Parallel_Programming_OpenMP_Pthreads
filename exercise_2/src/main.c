#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include "mutual_exclution.h"
#include "atomic_operations.h"


int main(int argc, char *argv[])
{
    int thread_count, iterations;

    config(argc, argv, &iterations, &thread_count);

    // printf(">>> Starting mutual_exclution...\n");
    mutual_exclution(thread_count, iterations, 0);
    // printf(">>> Starting atomic_operations...\n");
    atomic_operations(thread_count, iterations, 0);

    return EXIT_SUCCESS;
}
