#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include "ex1_OpenMP.h"

#ifdef _OPENMP
#include <omp.h>
#endif


int open_mp(long long int num_darts, int thread_count)
{
    long long int arrows;
    double x, y, squared_dist, total_time, start, end;

    omp_set_num_threads(thread_count);

    arrows = 0;
    start = omp_get_wtime();
#pragma omp parallel for reduction(+ : arrows)
    for (long long int throw = 0; throw < num_darts; throw++)
    {
        // printf("Thread %d of %d\n", omp_get_thread_num(), omp_get_num_threads());
        // printf("throw: %lld\n", throw);

        unsigned int seed = (unsigned int)time(NULL) ^ throw; // απλός seed
        x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0; // float in range -1 to 1
        y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        // printf("x: %f, y: %f\n", x, y);

        squared_dist = (x * x) + (y * y);

        if (squared_dist <= 1)
            arrows++;

    }
    end = omp_get_wtime();
    total_time = end - start;

    // printf("Arrows: %lld\n", arrows);
    // printf("Darts: %lld\n", num_darts);
    // printf("Parallel OpenMP process takes %f seconds to execute\n", total_time);
    printf("OpenMp,%d,%lld,%lld,%.6f\n", thread_count, num_darts, arrows, total_time);

    return EXIT_SUCCESS;
}
