#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "timer.h"
#include "omp_mat_vect_rand_split.h"

/*------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int thread_count;
    int m, n;
    double *A;
    double *x;
    double *y;
    int use_upper = 1;

    Get_args(argc, argv, &thread_count, &m, &n, &use_upper);

    A = malloc(m * n * sizeof(double));
    x = malloc(n * sizeof(double));
    y = malloc(m * sizeof(double));

#ifdef DEBUG
    Read_matrix("Enter the matrix", A, m, n);
    Print_matrix("We read", A, m, n);
    Read_vector("Enter the vector", x, n);
    Print_vector("We read", x, n);
#else
    Gen_matrix(A, m, n);
    /* Print_matrix("We generated", A, m, n); */
    Gen_vector(x, n);
/* Print_vector("We generated", x, n); */
#endif

    if (use_upper)
        Omp_mat_vect(A, x, y, m, n, thread_count);
    else
        Omp_mat_vect_full_matrix(A, x, y, m, n, thread_count);
#ifdef DEBUG
    Print_vector("The product is", y, m);
#else
/* Print_vector("The product is", y, m); */
#endif

    free(A);
    free(x);
    free(y);

    return 0;
} /* main */
