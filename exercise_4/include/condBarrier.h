#ifndef CONDBARRIER_H
#define CONDBARRIER_H

// Sets up and runs the barrier test.
void condition_barrier(int threads, int num_iterations);

// SIGINT handler
void handle_sigint_barrierCond(int);

void *test_barrier_cond(void *);

#endif // CONDBARRIER_H
