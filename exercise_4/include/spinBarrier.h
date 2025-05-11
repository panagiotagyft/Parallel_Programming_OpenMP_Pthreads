#ifndef SPINBARRIER_H
#define SPINBARRIER_H

#include <stdio.h>

void spin_barrier(int, int);

void *test_sense_reversal_centralized_barrier(void *);

void handle_sigint_barrierSence(int);

#endif // SPINBARRIER_H