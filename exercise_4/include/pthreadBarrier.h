#ifndef PTHREADBARRIER_H
#define PTHREADBARRIER_H

#include <stdio.h>

void pthread_barrier(int, int);

// Each thread waits on the barrier for a given number of iterations
void *test_barrier_pthread(void *);

void handle_sigint_barrierPthread(int);

#endif // PTHREADBARRIER_H