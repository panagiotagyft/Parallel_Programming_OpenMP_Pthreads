#ifndef WORK_ARRAY_H
#define WORK_ARRAY_H

#include <stdio.h>
#include <stdarg.h>

// Thread function that performs work with mutex-protected counter
void * culc_sum(void *);

// This function spawns threads, each of which increments a shared
// variable while using locks to avoid race conditions.
void work_array(int, int, int *, bool);

void handle_sigint_array(int);

#endif // WORK_ARRAY_H