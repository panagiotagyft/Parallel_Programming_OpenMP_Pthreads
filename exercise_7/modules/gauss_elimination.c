#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "gauss_elimination.h"

// -------------------------------------------------------------------------------------------
// -- Solves the upper triangular system using column/row-wise elimination (serial version) --
// -------------------------------------------------------------------------------------------

void column_wise_elimination(double **A, double *b, double *x, int n)
{
    for (int row = 0; row < n; row++)
        x[row] = b[row];
    for (int col = n - 1; col >= 0; col--)
    {
        x[col] /= A[col][col];
        for (int row = 0; row < col; row++)
            x[row] -= A[row][col] * x[col];
    }
}

void row_wise_elimination(double **A, double *b, double *x, int n)
{
    for (int row = n - 1; row >= 0; row--)
    {
        x[row] = b[row];
        for (int col = row + 1; col < n; col++)
            x[row] -= A[row][col] * x[col];
        x[row] /= A[row][row];
    }
}

// -------------------------------------------------------------------------------------------
// -- Solves the upper triangular system using column/row-wise elimination (parallel version) --
// -------------------------------------------------------------------------------------------

void parallel_column_wise_elimination(double **A, double *b, double *x, int n, int thread_count)
{
    omp_set_num_threads(thread_count); // Set the number of threads

    for (int row = 0; row < n; row++)
        x[row] = b[row];

    for (int col = n - 1; col >= 0; col--)
    {
        x[col] /= A[col][col];
#pragma omp parallel for
        for (int row = 0; row < col; row++)
            x[row] -= A[row][col] * x[col];
    }
}

void parallel_row_wise_elimination(double **A, double *b, double *x, int n, int thread_count)
{
    omp_set_num_threads(thread_count);

    for (int row = n - 1; row >= 0; row--)
    {
        double sum = 0.0;
#pragma omp parallel for reduction(+ : sum)
        for (int col = row + 1; col < n; col++)
            sum += A[row][col] * x[col];

        x[row] = (b[row] - sum) / A[row][row];
    }
}