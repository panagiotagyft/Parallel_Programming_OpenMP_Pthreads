/* File:     
 *     omp_mat_vect_rand_split.c 
 *
 * Purpose:  
 *     Computes a parallel matrix-vector product.  Matrix
 *     is distributed by block rows.  Vectors are distributed by 
 *     blocks.  This version uses a random number generator to
 *     generate A and x.  There is some optimization.
 *
 * Compile:  
 *    gcc -g -Wall -fopenmp -o omp_mat_vect_rand_split 
 *          omp_mat_vect_rand_split.c 
 * Run:
 *    ./omp_mat_vect_rand_split <thread_count> <m> <n>
 *
 * Input:
 *     None unless compiled with DEBUG flag.
 *     With DEBUG flag, A, x
 *
 * Output:
 *     y: the product vector
 *     Elapsed time for the computation
 *
 * Notes:  
 *     1.  Storage for A, x, y is dynamically allocated.
 *     2.  Number of threads (thread_count) should evenly divide both 
 *         m and n.  The program doesn't check for this.
 *     3.  We use a 1-dimensional array for A and compute subscripts
 *         using the formula A[i][j] = A[i*n + j]
 *     4.  Distribution of A, x, and y is logical:  all three are 
 *         globally shared.
 *     5.  DEBUG compile flag will prompt for input of A, x, and
 *         print y
 *
 * IPP:  Exercise 5.12
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "timer.h"
#include "omp_mat_vect_rand_split.h"

/*------------------------------------------------------------------
 * Function:  Get_args
 * Purpose:   Get command line args
 * In args:   argc, argv
 * Out args:  thread_count_p, m_p, n_p
 */
void Get_args(int argc, char *argv[], int *thread_count_p, int *m_p, int *n_p, int *use_upper)
{
   // Default: use upper triangular optimization
   *use_upper = 1;

   // Αν υπάρχει 5ο όρισμα και είναι "full", απενεργοποιούμε την upper optimization
   if (argc == 5 && strcmp(argv[4], "full") == 0)
      *use_upper = 0;

   // Δεκτό μόνο αν έχουμε 4 ή 5 ορίσματα
   if (argc != 4 && argc != 5)
      Usage(argv[0]);

   *thread_count_p = strtol(argv[1], NULL, 10);
   *m_p = strtol(argv[2], NULL, 10);
   *n_p = strtol(argv[3], NULL, 10);

   if (*thread_count_p <= 0 || *m_p <= 0 || *n_p <= 0)
      Usage(argv[0]);
}

/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   print a message showing what the command line should
 *            be, and terminate
 * In arg :   prog_name
 */
void Usage (char* prog_name) {
   fprintf(stderr, "usage: %s <thread_count> <m> <n>\n", prog_name);
   exit(0);
}  /* Usage */

/*------------------------------------------------------------------
 * Function:    Read_matrix
 * Purpose:     Read in the matrix
 * In args:     prompt, m, n
 * Out arg:     A
 */
void Read_matrix(char* prompt, double A[], int m, int n) {
   int             i, j;

   printf("%s\n", prompt);
   for (i = 0; i < m; i++) 
      for (j = 0; j < n; j++)
         scanf("%lf", &A[i*n+j]);
}  /* Read_matrix */

/*------------------------------------------------------------------
 * Function: Gen_matrix
 * Purpose:  Use the random number generator random to generate
 *    the entries in A
 * In args:  m, n
 * Out arg:  A
 */
void Gen_matrix(double A[], int m, int n) {
   int i, j;
   for (i = 0; i < m; i++)
      for (j = 0; j < n; j++)
         A[i*n+j] = random()/((double) RAND_MAX);
}  /* Gen_matrix */

/*------------------------------------------------------------------
 * Function: Gen_vector
 * Purpose:  Use the random number generator random to generate
 *    the entries in x
 * In arg:   n
 * Out arg:  A
 */
void Gen_vector(double x[], int n) {
   int i;
   for (i = 0; i < n; i++)
      x[i] = random()/((double) RAND_MAX);
}  /* Gen_vector */

/*------------------------------------------------------------------
 * Function:        Read_vector
 * Purpose:         Read in the vector x
 * In arg:          prompt, n
 * Out arg:         x
 */
void Read_vector(char* prompt, double x[], int n) {
   int   i;

   printf("%s\n", prompt);
   for (i = 0; i < n; i++) 
      scanf("%lf", &x[i]);
}  /* Read_vector */


/*------------------------------------------------------------------
 * Function:  Omp_mat_vect
 * Purpose:   Multiply an mxn matrix by an nx1 column vector
 * In args:   A, x, m, n, thread_count
 * Out arg:   y
 */
void Omp_mat_vect(double A[], double x[], double y[],
      int m, int n, int thread_count) {
   int i, j;
   double start, finish, elapsed, temp;

   start=omp_get_wtime();
#pragma omp parallel for num_threads(thread_count) \
    schedule(runtime) \
    default(none) private(i, j, temp) shared(A, x, y, m, n)
   for (i = 0; i < m; i++) {
      y[i] = 0.0;
      for (j = i; j < n; j++) {
         temp = A[i*n+j]*x[j];
         y[i] += temp;
	  }
   }
   
   finish = omp_get_wtime();
   elapsed = finish - start;
//   printf("Elapsed time = %e seconds\n", elapsed);
   printf("%e\n", elapsed);

}  /* Omp_mat_vect */

void Omp_mat_vect_full_matrix(double A[], double x[], double y[], int m, int n, int thread_count) {
   int i, j;
   double start, finish, elapsed, temp;

   start=omp_get_wtime();
#  pragma omp parallel for num_threads(thread_count)  \
      default(none) private(i, j, temp)  shared(A, x, y, m, n)
   for (i = 0; i < m; i++) {
      y[i] = 0.0;
      for (j = 0; j < n; j++) {
         temp = A[i*n+j]*x[j];
         y[i] += temp;
      }
   }

   finish = omp_get_wtime();
   elapsed = finish - start;
//   printf("Elapsed time = %e seconds\n", elapsed);
   printf("%e\n", elapsed);

}  /* Omp_mat_vect_1st */


/*------------------------------------------------------------------
 * Function:    Print_matrix
 * Purpose:     Print the matrix
 * In args:     title, A, m, n
 */
void Print_matrix( char* title, double A[], int m, int n) {
   int   i, j;

   printf("%s\n", title);
   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++)
         printf("%4.1f ", A[i*n + j]);
      printf("\n");
   }
}  /* Print_matrix */


/*------------------------------------------------------------------
 * Function:    Print_vector
 * Purpose:     Print a vector
 * In args:     title, y, m
 */
void Print_vector(char* title, double y[], double m) {
   int   i;

   printf("%s\n", title);
   for (i = 0; i < m; i++)
      printf("%4.1f ", y[i]);
   printf("\n");
}  /* Print_vector */
