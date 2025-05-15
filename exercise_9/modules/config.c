#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "config.h"

// Function to configure program parameters based on command-line arguments
void config(int argc, char *argv[], int *generations, int *n, int *parallel_impl, int *thread_count)
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
        if (strcmp(argv[1], "-g") > 0)
        {
            printf("Incorrect argument: %s .The correct argument is: -g (generations)\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        if (strcmp(argv[3], "-gs") > 0)
        {
            printf("Incorrect argument: %s .The correct argument is: -gs (grid size)\n", argv[3]);
            exit(EXIT_FAILURE);
        }
        if (strcmp(argv[5], "-impl") > 0)
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

        // Convert string arguments to integers
        *generations = atoi(argv[2]);
        *n = atoi(argv[4]);
        *parallel_impl = atoi(argv[6]);
        *thread_count = atoi(argv[8]);

        // Check for valid parallel implementation value
        if (*parallel_impl != 0 && *parallel_impl != 1)
        {
            write(STDOUT_FILENO, "6th argument must be 0 (serial) or 1 (parallel).\n", 49);
            exit(EXIT_FAILURE);
        }

        // Check that thread count is greater than 0 if parallel mode is selected
        if (*parallel_impl == 1 && *thread_count == 0)
        {
            write(STDOUT_FILENO, "8th argument must be greater than 1.\n", 37);
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
