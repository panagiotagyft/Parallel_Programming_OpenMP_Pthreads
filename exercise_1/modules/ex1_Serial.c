#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ex1_Serial.h"

int serial(long long int num_darts)
{
    long long int arrows;
    double x, y, squared_dist, total_time, start, end;

    arrows = 0;
    start = ((double)clock()) / CLOCKS_PER_SEC;
    for (long long int throw = 0; throw < num_darts; throw ++)
    {
        // printf("Thread %d of %d\n", omp_get_thread_num(), omp_get_num_threads());
        // printf("throw: %lld\n", throw);

        unsigned int seed = (unsigned int)time(NULL) ^ throw; // απλός seed για αποσυμφόρηση
        x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;     // float in range -1 to 1
        y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        // printf("x: %f, y: %f\n", x, y);

        squared_dist = (x * x) + (y * y);

        if (squared_dist <= 1)
            arrows++;
    }
    end = ((double)clock()) / CLOCKS_PER_SEC;
    total_time = end - start;

    // printf("Arrows: %lld\n", arrows);
    // printf("Darts: %lld\n", num_darts);
    // printf("Serial process takes %f seconds to execute\n", total_time);
    printf("Serial,1,%lld,%lld,%.6f\n", num_darts, arrows, total_time);

    return EXIT_SUCCESS;
}
