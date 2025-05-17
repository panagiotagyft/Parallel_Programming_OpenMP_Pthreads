#ifndef OMP_MAT_VECT_RAND_SPLIT_H
#define OMP_MAT_VECT_RAND_SPLIT_H

/* Serial functions */
void Get_args(int argc, char *argv[], int *thread_count_p,
              int *m_p, int *n_p, int *use_upper);
void Usage(char* prog_name);
void Gen_matrix(double A[], int m, int n);
void Read_matrix(char* prompt, double A[], int m, int n);
void Gen_vector(double x[], int n);
void Read_vector(char* prompt, double x[], int n);
void Print_matrix(char* title, double A[], int m, int n);
void Print_vector(char* title, double y[], double m);
void Run_all_schedules(double A[], double x[], double y[], int, int, int);
/* Parallel function */
void Omp_mat_vect(double A[], double x[], double y[],
    int m, int n, int thread_count);
void Omp_mat_vect_full_matrix(double A[], double x[], double y[],
    int m, int n, int thread_count);

#endif