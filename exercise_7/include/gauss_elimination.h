#ifndef GAUSS_ELIMINATION_H
#define GAUSS_ELIMINATION_H

// Solves the upper triangular system using column/row-wise elimination (serial version)
void column_wise_elimination(double **, double *, double *, int);
void row_wise_elimination(double **, double *, double *, int);

// Solves the upper triangular system using column/row-wise elimination (parallel version)
void parallel_column_wise_elimination(double **, double *, double *, int, int);
void parallel_row_wise_elimination(double **, double *, double *, int, int);

#endif // GAUSS_ELIMINATION_H