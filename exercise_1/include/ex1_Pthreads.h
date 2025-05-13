#ifndef EX1_PTHREADS_H
#define EX1_PTHREADS_H

#include <stdio.h>

int pthreads_impl(long long int, int);
void handle_sigint_mutex(int);
void *thread_func(void *);

#endif // EX1_PTHREADS_H