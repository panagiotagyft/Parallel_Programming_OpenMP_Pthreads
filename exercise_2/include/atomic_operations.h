#ifndef ATOMIC_OPERATIONS_H
#define ATOMIC_OPERATIONS_H

#include <stdio.h>

// This function spawns threads, each of which increments a shared
// variable while using locks to avoid race conditions.
void atomic_operations(int, int);

void handle_sigint_atomic(void);

#endif // ATOMIC_OPERATIONS_H

