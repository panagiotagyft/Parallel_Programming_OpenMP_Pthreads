#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <stdio.h>

void gameInitialization(int, int **, int **);

void serial_game_of_life(int, int, int **, int **);
void parallel_game_of_life(int, int, int **, int **);

#endif // GAME_OF_LIFE_H