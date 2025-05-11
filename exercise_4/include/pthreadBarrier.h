#ifndef PTHREADBARRIER_H
#define PTHREADBARRIER_H

#include <stdio.h>

void pthread_barrier(int, int);

void *test_barrier_pthread(void *);

void handle_sigint_barrierPthread(int);

#endif // PTHREADBARRIER_H