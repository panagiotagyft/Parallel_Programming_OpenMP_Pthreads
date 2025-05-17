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
    int thread_count, iterations, val;

    config(argc, argv, &iterations, &thread_count);

    val = mutual_exclution(thread_count, iterations, 0);

    val = atomic_operations(thread_count, iterations, 0);

    (void)val;
    // printf("val: %d\n", val);

    return EXIT_SUCCESS;
}
