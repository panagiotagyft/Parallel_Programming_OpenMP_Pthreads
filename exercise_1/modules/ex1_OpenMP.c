#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include "ex1_OpenMP.h" 

#ifdef _OPENMP
#include <omp.h> // OpenMP API for parallel timing and threading
#endif

int open_mp(long long int num_darts, int thread_count)
{
    long long int arrows = 0;
    double start, end, total_time;

    // Set the number of threads for the OpenMP parallel region
    omp_set_num_threads(thread_count);
   
    start = omp_get_wtime();

#pragma omp parallel
    {
        // Each thread must initialize its own seed to avoid contention and correlation
        unsigned int seed = (unsigned int)time(NULL) ^ omp_get_thread_num();
        
#pragma omp for reduction(+ : arrows) 
        for (long long int throw = 0; throw < num_darts; ++throw)
        {
            // Generate a random point (x, y) in the square [-1, 1] x [-1, 1]
            double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
            double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;

            // Check if the point lies inside the unit circle (x^2 + y^2 <= 1)
            if (x * x + y * y <= 1.0)
                arrows++;
        }
    } // end parallel
   
    end = omp_get_wtime();
    total_time = end - start;
    double pi = 4 * arrows / (double)num_darts;
    printf("OpenMp,%d,%lld,%lld,%.6f,%lf\n", thread_count, num_darts, arrows, total_time, pi);

    return EXIT_SUCCESS;
}
