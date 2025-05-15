#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "gauss_elimination.h"
#include "config.h"

#ifdef _OPENMP
#include <omp.h>
#endif

void initialize_system(double **, double *, int);

int main(int argc, char *argv[])
{
    long int n;
    int parallel_impl, row_or_col, thread_count, i, j;
    double total_time, start, end;
    double **A, *b, *x;

    srand(10);

    // Read configuration from command-line arguments
    config(argc, argv, &parallel_impl, &row_or_col, &thread_count, &n);
    
    // Allocate memory for matrix A and vectors b, x
    A = calloc(n, sizeof(double *));
    b = calloc(n, sizeof(double));
    x = calloc(n, sizeof(double));
    if (A == NULL || b == NULL || x == NULL)
    {
        write(STDERR_FILENO, "ERROR Not available memory\n", 27);
        return EXIT_FAILURE;
    }

    // Allocate memory for each row of matrix A
    for (i = 0; i < n; i++)
    {
        A[i] = calloc(n, sizeof(double));
        if (A[i] == NULL)
        {
            write(STDERR_FILENO, "ERROR Not available memory\n", 27);

            for (j = 0; j < i; j++)
                free(A[j]);

            free(A);

            return EXIT_FAILURE;
        }
    }

    // Initialize A as an upper triangular matrix and vector b with random values
    initialize_system(A, b, n);

    if(parallel_impl == 0)
    { // Serial execution

        start = ((double)clock()) / CLOCKS_PER_SEC;
        if (row_or_col == 0)
            row_wise_elimination(A, b, x, n);
        else
            column_wise_elimination(A, b, x, n);
        
        end = ((double)clock()) / CLOCKS_PER_SEC;
        total_time = end - start;
        printf("%ld,serial,%s,%f\n", n, row_or_col == 0 ? "row wise" : "column wise", total_time);

    }
    else
    { // Parallel execution
      
        start = omp_get_wtime();
        if (row_or_col == 0)
            parallel_row_wise_elimination(A, b, x, n, thread_count);
        else
            parallel_column_wise_elimination(A, b, x, n, thread_count);

        end = omp_get_wtime();
        total_time = end - start;

        printf("%ld,%d,%s,%f\n", n, thread_count, row_or_col == 0 ? "row wise" : "column wise", total_time);
    }

    for (j = 0; j < n; j++)
        free(A[j]);

    free(A);
    free(b);
    free(x);

    return EXIT_SUCCESS;
}

// Function to generate an upper triangular matrix A with random values and random vector b
void initialize_system(double **A, double *b, int n)
{
    for (int i = 0; i < n; i++)
    {
        b[i] = rand() % 100 + 1; 

        for (int j = 0; j < n; j++)
        {
            // Diagonal elements
            if (j == i)
            {
                A[i][j] = (rand() % 50 + 50);
                continue;
            }

            // Upper triangular part
            if (j > i)
                A[i][j] = rand() % 100;
        }
    }
}