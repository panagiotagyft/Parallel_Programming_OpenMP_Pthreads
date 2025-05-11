#ifndef CONDBARRIER_H
#define CONDBARRIER_H

/* Κάνει barrier με mutex+cond, threads φορές */
void condition_barrier(int threads, int num_iterations);

/* SIGINT handler */
void handle_sigint_barrierCond(int signum);

#endif // CONDBARRIER_H
