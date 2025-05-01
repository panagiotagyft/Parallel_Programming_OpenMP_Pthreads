#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#ifdef _OPENMP
#include <omp.h>
#endif

bool isnumber(const char *);

int main(int argc, char *argv[])
{
    long long int arrows, num_darts;
    double x, y, squared_dist, total_time, start, end;
    int thread_count;

    srand(time(NULL));

    if (argc < 3)
    {
        write(STDOUT_FILENO, "Missing arguments...\n", 21);
        return EXIT_FAILURE;
    }
    else if (argc > 3)
    {
        write(STDOUT_FILENO, "Too many arguments...\n", 22);
        return EXIT_FAILURE;
    }
    else
    {
        if (!isnumber(argv[1]) || !isnumber(argv[2]))
        {
            write(STDOUT_FILENO, "Arguments must be positive integers!!\n", 38);
            return EXIT_FAILURE;
        }

        num_darts = atoi(argv[1]);
        thread_count = atoi(argv[2]);
    }

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

    printf("Arrows: %lld\n", arrows);
    printf("Darts: %lld\n", num_darts);
    printf("Parallel OpenMP process takes %f seconds to execute\n", total_time);

    
    return EXIT_SUCCESS;
}

bool isnumber(const char *str)
{
    if (*str == '\0')
        return false;

    do
    {
        if (*str < '0' || *str > '9')
            return false;
        str++;
    } while (*str);

    return true;
}
