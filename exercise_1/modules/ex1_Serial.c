#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ex1_Serial.h"

int serial(long long int num_darts)
{
    long long int arrows;
    double x, y, squared_dist, total_time, start, end;

    arrows = 0; // Initialize hit counter
    
    start = ((double)clock()) / CLOCKS_PER_SEC;
    
    // Loop over the total number of dart throws
    for (long long int throw = 0; throw < num_darts; throw ++)
    {
        // Generate a seed value based on current time and iteration index
        // This helps diversify the random sequences between iterations
        unsigned int seed = (unsigned int)time(NULL) ^ throw;

        // Generate a random x-coordinate in the range [-1.0, 1.0]
        x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;     
        y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;

        // Compute the squared distance from the origin (0,0)
        squared_dist = (x * x) + (y * y);

        // If the point lies within the unit circle, count it as a hit
        if (squared_dist <= 1)
            arrows++;
    }
    double pi = 4 * arrows / (double)num_darts;
    end = ((double)clock()) / CLOCKS_PER_SEC;
    total_time = end - start;

    printf("Serial,1,%lld,%lld,%.6f,%lf\n", num_darts, arrows, total_time, pi);

    return EXIT_SUCCESS;
}
