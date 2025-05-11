#ifndef MUTUAL_EXCLUTION_H
#define MUTUAL_EXCLUTION_H

#include <stdio.h>

// Thread function that performs work with mutex-protected counter
void * culc_sum(void *);

// This function spawns threads, each of which increments a shared
// variable while using locks to avoid race conditions.
void mutual_exclution(int, int);

void handle_sigint_mutex(void);

#endif // MUTUAL_EXCLUTION_H