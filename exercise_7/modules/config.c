#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "config.h"

// Function to configure program parameters based on command-line arguments
void config(int argc, char *argv[], int *parallel_impl, int *row_or_col, int *thread_count, long int *n)
{
    if (argc < 9)
    {
        write(STDOUT_FILENO, "Missing arguments...\n", 21);
        exit(EXIT_FAILURE);
    }
    else if (argc > 9)
    {
        write(STDOUT_FILENO, "Too many arguments...\n", 22);
        exit(EXIT_FAILURE);
    }
    else
    {
        if (strcmp(argv[1], "-d") > 0)
        {
            printf("Incorrect argument: %s .The correct argument is: -g (generations)\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        if (strcmp(argv[3], "-sp") > 0)
        {
            printf("Incorrect argument: %s .The correct argument is: -gs (grid size)\n", argv[3]);
            exit(EXIT_FAILURE);
        }
        if (strcmp(argv[5], "-rc") > 0)
        {
            printf("Incorrect argument: %s .The correct argument is: -impl (parallel implementation true/false)\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        if (strcmp(argv[7], "-t") > 0)
        {
            printf("Incorrect argument: %s .The correct argument is: -t (threads)\n", argv[3]);
            exit(EXIT_FAILURE);
        }
        if (!isnumber(argv[2]) || !isnumber(argv[4]) || !isnumber(argv[6]) || !isnumber(argv[8]))
        {
            write(STDOUT_FILENO, "Arguments must be positive integers!!\n", 38);
            exit(EXIT_FAILURE);
        }
  
        if (!isnumber(argv[2]) || !isnumber(argv[4]) || !isnumber(argv[6]) || !isnumber(argv[8]))
        {
            write(STDOUT_FILENO, "Arguments must be positive integers!!\n", 38);
            exit(EXIT_FAILURE);
        }

        // Convert string arguments to integers
        *n = atoi(argv[2]);
        *parallel_impl = atoi(argv[4]);
        *row_or_col = atoi(argv[6]);
        *thread_count = atoi(argv[8]);

        // Check for valid parallel implementation value
        if (*parallel_impl != 0 && *parallel_impl != 1)
        {
            write(STDOUT_FILENO, "Second argument must be 0 (serial) or 1 (parallel).\n", 52);
            exit(EXIT_FAILURE);
        }

        // Check for valid row/column selection
        if (*row_or_col != 0 && *row_or_col != 1)
        {
            write(STDOUT_FILENO, "Third argument must be 0 (row-wise) or 1 (column-wise).\n", 57);
            exit(EXIT_FAILURE);
        }

        // Check that thread count is greater than 0 if parallel mode is selected
        if (*parallel_impl == 1 && *thread_count == 0)
        {
            write(STDOUT_FILENO, "Fifth argument must be greater than 1.\n", 41);
            exit(EXIT_FAILURE);
        }
    }
}

// Helper function to check if a string is a number (contains only digits)
bool isnumber(const char *str)
{
    if (*str == '\0')
        return false;

    do
    {
        if (*str < '0' || *str > '9')
            return false;
        str++;
    } while (*str);

    return true;
}
