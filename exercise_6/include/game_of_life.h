#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <stdio.h>

// Count the number of live neighbors of cell (i, j) in the grid
int count_neighbors(int **, int, int, int);

// Randomly initialize the grid with live cells
void gameInitialization(int, int **, int **);

// Serial implementation of the Game of Life
void serial_game_of_life(int, int, int **, int **);

// Parallel implementation using OpenMP collapse
void parallel_game_of_life(int, int, int **, int **);

#endif // GAME_OF_LIFE_H