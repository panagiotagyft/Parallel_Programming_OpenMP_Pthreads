#ifndef EX1_OPENMP_H
#define EX1_OPENMP_H

#include <stdio.h>

void handle_sigint_mutex(int);
int open_mp(long long int, int);
void *thread_func(void *);

#endif // EX1_OPENMP_H