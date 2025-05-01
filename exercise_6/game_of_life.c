#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

int main(void)
{
    int n = 2, i, j, generations, neighbors;
    int **grid;

    srand(time(NULL));

    // allocate pointer array
    grid = calloc(n, sizeof(int *));
    next_grid = calloc(n, sizeof(int *));
    if (grid == NULL || next_grid == NULL)
    {
        // γράψε μήνυμα λάθους και τερματισμός
        write(STDERR_FILENO, "ERROR Not available memory\n", 27);
        return EXIT_FAILURE;
    }

    // allocate each row
    for (i = 0; i < n; i++)
    {
        grid[i] = calloc(n, sizeof(int));
        next_grid[i] = calloc(n, sizeof(int));
        if (grid[i] == NULL || next_grid[i] == NULL)
        {
            write(STDERR_FILENO, "ERROR Not available memory\n", 27);
            // απελευθέρωσε όσα ήδη είχες κάνει calloc
            for (j = 0; j < i; j++)
                free(grid[j]);
                free(next_grid[j]);

            free(grid);
            free(next_grid);
            return EXIT_FAILURE;
        }
    }
    int live_cells = n + rand() % (n * n - n + 1);
    printf("test %d\n", live_cells);
    for (int cell = 0; cell < live_cells; cell++)
    {
        for (;;)
        {
            i = rand() % n;
            j = rand() % n;
            if (grid[i][j] == 0)
                break;
        }

        grid[i][j] = 1;
        next_grid[i][j] = 1;
    }

    for (int gen = 0; gen < generations; gen++)
    {

#pragma omp parallel for collapse(2)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                if (i == 0 && j == 0)
                    neighbors = grid[i + 1][j] + grid[i + 1][j + 1] + grid[i][j + 1];
                else if (i == 0)
                    neighbors = grid[i + 1][j] + grid[i + 1][j + 1] + grid[i + 1][j - 1] + grid[i][j - 1] + grid[i][j + 1];
                else if (j == 0)
                    neighbors = grid[i - 1][j] + grid[i + 1][j] + grid[i + 1][j + 1] + grid[i - 1][j + 1] + grid[i][j + 1];
                else
                    neighbors = grid[i - 1][j] + grid[i + 1][j] + grid[i + 1][j + 1] + grid[i + 1][j - 1] + grid[i - 1][j + 1] + grid[i - 1][j - 1] + grid[i][j - 1] + grid[i][j + 1];

                if (grid[i][j] == 1 && (neighbors < 2 || neighbors > 3))
                    next_grid[i][j] = 0;

                if (grid[i][j] == 0 && neighbors == 3)
                    next_grid[i][j] = 1;
            }

#pragma omp parallel for collapse(2)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                grid[i][j] = next_grid[i][j];
    }
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%d ", grid[i][j]);

        printf("\n");
    }

    for (int i = 0; i < n; i++)
        free(grid[i]);
    free(grid);

    return EXIT_SUCCESS;
}