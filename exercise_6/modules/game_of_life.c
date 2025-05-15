#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>

#ifdef _OPENMP
#include <omp.h> // Include OpenMP for parallel execution
#endif

// Count the number of live neighbors of cell (i, j) in the grid
int count_neighbors(int **grid, int n, int i, int j)
{
    int neighbors = 0;

    // Check each case based on the cell's position in the grid

    // Top-left corner
    if (i == 0 && j == 0)
        neighbors = grid[i][j + 1] + grid[i + 1][j] + grid[i + 1][j + 1];

    // Top-right corner
    else if (i == 0 && j == n - 1)
        neighbors = grid[i][j - 1] + grid[i + 1][j] + grid[i + 1][j - 1];

    // Bottom-left corner
    else if (i == n - 1 && j == 0)
        neighbors = grid[i - 1][j] + grid[i - 1][j + 1] + grid[i][j + 1];

    // Bottom-right corner
    else if (i == n - 1 && j == n - 1)
        neighbors = grid[i][j - 1] + grid[i - 1][j] + grid[i - 1][j - 1];

    // Top edge (excluding corners)
    else if (i == 0)
        neighbors = grid[i][j - 1] + grid[i][j + 1] +
                    grid[i + 1][j - 1] + grid[i + 1][j] + grid[i + 1][j + 1];

    // Bottom edge (excluding corners)
    else if (i == n - 1)
        neighbors = grid[i][j - 1] + grid[i][j + 1] +
                    grid[i - 1][j - 1] + grid[i - 1][j] + grid[i - 1][j + 1];

    // Left edge (excluding corners)
    else if (j == 0)
        neighbors = grid[i - 1][j] + grid[i + 1][j] +
                    grid[i - 1][j + 1] + grid[i][j + 1] + grid[i + 1][j + 1];

    // Right edge (excluding corners)
    else if (j == n - 1)
        neighbors = grid[i - 1][j] + grid[i + 1][j] +
                    grid[i - 1][j - 1] + grid[i][j - 1] + grid[i + 1][j - 1];

    // Internal cell (not on edge or corner)
    else
        neighbors = grid[i - 1][j - 1] + grid[i - 1][j] + grid[i - 1][j + 1] +
                    grid[i][j - 1] + grid[i][j + 1] +
                    grid[i + 1][j - 1] + grid[i + 1][j] + grid[i + 1][j + 1];

    return neighbors;
}

// Randomly initialize the grid with live cells
void gameInitialization(int n, int **grid, int **next_grid)
{
    int live_cells = n + rand() % (n * n - n + 1); // Random number of initial live cells
    for (int cell = 0; cell < live_cells;)
    {
        int i = rand() % n;
        int j = rand() % n;
        if (grid[i][j] == 0) // Place a live cell if the position is empty
        {
            grid[i][j] = 1;
            next_grid[i][j] = 1;
            cell++;
        }
    }
}

// Serial implementation of the Game of Life
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

                // Apply Conway's Game of Life rules
                if (grid[i][j] == 1 && (neighbors < 2 || neighbors > 3))
                    next_grid[i][j] = 0; // Dies due to under/overpopulation
                else if (grid[i][j] == 0 && neighbors == 3)
                    next_grid[i][j] = 1; // Becomes alive due to reproduction
                else
                    next_grid[i][j] = grid[i][j]; // State remains unchanged
            }
        }

        // Copy updated grid to the current grid
        for (int i = 0; i < n; i++)
            memcpy(grid[i], next_grid[i], n * sizeof(int));
    }
}

// Parallel implementation using OpenMP collapse
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

        // Copy next_grid to grid in parallel
#pragma omp parallel for collapse(2)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                grid[i][j] = next_grid[i][j];
    }
}
