#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "config.h"
#include "game_of_life.h"

#ifdef _OPENMP
#include <omp.h>
#endif

void memoryAllocation(int n, int ***, int ***);

int main(int argc, char *argv[])
{
    int n, generations, parallel_impl, thread_count;
    int **grid, **next_grid;
    double total_time, start, end;

    srand(10);

    config(argc, argv, &generations, &n, &parallel_impl, &thread_count);
    
    memoryAllocation(n, &grid, &next_grid);

    gameInitialization(n, grid, next_grid);

    if (parallel_impl == 0)
    { // Serial execution

        start = ((double)clock()) / CLOCKS_PER_SEC;

        serial_game_of_life(generations, n, grid, next_grid);

        end = ((double)clock()) / CLOCKS_PER_SEC;
        total_time = end - start;
        printf("%d,serial,%d,%f\n", n, generations, total_time);

        // printf("Serial process takes %f seconds to execute\n", total_time);
    }
    else{
        start = omp_get_wtime();
        omp_set_num_threads(thread_count);
        parallel_game_of_life(generations, n, grid, next_grid);

        end = omp_get_wtime();
        total_time = end - start;

        printf("%d,%d,%d,%f\n", n, thread_count, generations, total_time);
    }

    
    for (int i = 0; i < n; i++)
    {
        free(grid[i]);
        free(next_grid[i]);
    }
    free(grid);
    free(next_grid);

    return EXIT_SUCCESS;
}

void memoryAllocation(int n, int ***grid, int ***next_grid)
{

    *grid = calloc(n, sizeof(int *));
    *next_grid = calloc(n, sizeof(int *));
    if (*grid == NULL || *next_grid == NULL)
    {
        write(STDERR_FILENO, "ERROR Not available memory\n", 27);
        exit(EXIT_FAILURE);
    }

    // allocate each row
    for (int i = 0; i < n; i++)
    {
        (*grid)[i] = calloc(n, sizeof(int));
        (*next_grid)[i] = calloc(n, sizeof(int));
        if ((*grid)[i] == NULL || (*next_grid)[i] == NULL)
        {
            write(STDERR_FILENO, "ERROR Not available memory\n", 27);
            for (int j = 0; j < i; j++)
            {
                free((*grid)[j]);
                free((*next_grid)[j]);
            }

            free(*grid);
            free(*next_grid);

            exit(EXIT_FAILURE);
        }
    }
}