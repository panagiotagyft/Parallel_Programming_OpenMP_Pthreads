#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

int count_neighbors(int **grid, int n, int i, int j)
{
    int neighbors = 0;

    // Corners
    if (i == 0 && j == 0)
        neighbors = grid[i][j + 1] + grid[i + 1][j] + grid[i + 1][j + 1];
    else if (i == 0 && j == n - 1)
        neighbors = grid[i][j - 1] + grid[i + 1][j] + grid[i + 1][j - 1];
    else if (i == n - 1 && j == 0)
        neighbors = grid[i - 1][j] + grid[i - 1][j + 1] + grid[i][j + 1];
    else if (i == n - 1 && j == n - 1)
        neighbors = grid[i][j - 1] + grid[i - 1][j] + grid[i - 1][j - 1];

    // Edges
    else if (i == 0)
        neighbors = grid[i][j - 1] + grid[i][j + 1] +
                    grid[i + 1][j - 1] + grid[i + 1][j] + grid[i + 1][j + 1];
    else if (i == n - 1)
        neighbors = grid[i][j - 1] + grid[i][j + 1] +
                    grid[i - 1][j - 1] + grid[i - 1][j] + grid[i - 1][j + 1];
    else if (j == 0)
        neighbors = grid[i - 1][j] + grid[i + 1][j] +
                    grid[i - 1][j + 1] + grid[i][j + 1] + grid[i + 1][j + 1];
    else if (j == n - 1)
        neighbors = grid[i - 1][j] + grid[i + 1][j] +
                    grid[i - 1][j - 1] + grid[i][j - 1] + grid[i + 1][j - 1];

    // Inner cells
    else
        neighbors = grid[i - 1][j - 1] + grid[i - 1][j] + grid[i - 1][j + 1] +
                    grid[i][j - 1] + grid[i][j + 1] +
                    grid[i + 1][j - 1] + grid[i + 1][j] + grid[i + 1][j + 1];

    return neighbors;
}

void gameInitialization(int n, int **grid, int **next_grid)
{
    int live_cells = n + rand() % (n * n - n + 1);

    for (int cell = 0; cell < live_cells;)
    {
        int i = rand() % n;
        int j = rand() % n;
        if (grid[i][j] == 0)
        {
            grid[i][j] = 1;
            next_grid[i][j] = 1;
            cell++;
        }
    }
}

void serial_game_of_life(int generations, int n, int **grid, int **next_grid)
{
    int neighbors;

    for (int gen = 0; gen < generations; gen++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                neighbors = count_neighbors(grid, n, i, j);

                // Game of Life rules
                if (grid[i][j] == 1 && (neighbors < 2 || neighbors > 3))
                    next_grid[i][j] = 0;
                else if (grid[i][j] == 0 && neighbors == 3)
                    next_grid[i][j] = 1;
                else
                    next_grid[i][j] = grid[i][j]; // remains the same
            }
        }

        // Copy next_grid to grid
        for (int i = 0; i < n; i++)
            memcpy(grid[i], next_grid[i], n * sizeof(int));
    }
}

void parallel_game_of_life(int generations, int n, int **grid, int **next_grid)
{
    int neighbors;

    for (int gen = 0; gen < generations; gen++)
    {

#pragma omp parallel for collapse(2)
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                neighbors = count_neighbors(grid, n, i, j);

                if (grid[i][j] == 1 && (neighbors < 2 || neighbors > 3))
                    next_grid[i][j] = 0;

                if (grid[i][j] == 0 && neighbors == 3)
                    next_grid[i][j] = 1;
            }
        }

#pragma omp parallel for collapse(2)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                grid[i][j] = next_grid[i][j];
    }
}
