#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "config.h"
#include <unistd.h>

// Function to configure program parameters based on command-line arguments
void config(int argc, char *argv[], int *iterations, int *thread_count)
{
    if (argc < 5)
    {
        write(STDOUT_FILENO, "Missing arguments...\n", 21);
        exit(EXIT_FAILURE);
    }
    else if (argc > 5)
    {
        write(STDOUT_FILENO, "Too many arguments...\n", 22);
        exit(EXIT_FAILURE);
    }
    else
    {
        if (strcmp(argv[1], "-i") > 0)
        {
            printf("Incorrect argument: %s .The correct argument is: -i (iterations)\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        if (strcmp(argv[3], "-t") > 0)
        {
            printf("Incorrect argument: %s .The correct argument is: -t (threads)\n", argv[3]);
            exit(EXIT_FAILURE);
        }

        // Ensure that both values (iterations and threads) are positive integers
        if (!isnumber(argv[2]) || !isnumber(argv[4]))
        {
            write(STDOUT_FILENO, "Arguments must be positive integers!!\n", 38);
            exit(EXIT_FAILURE);
        }

        // Convert string arguments to integers
        *iterations = atoi(argv[2]);
        *thread_count = strtol(argv[4], NULL, 10);

        // Ensure thread count is within valid bounds
        long nprocs = sysconf(_SC_NPROCESSORS_ONLN);
        if (nprocs < 1)
        {
            perror("Could not determine number of processors");
            exit(EXIT_FAILURE);
        }
        // printf("Available logical cores: %ld\n", nprocs);

        if (*thread_count <= 1 || *thread_count > nprocs)
        {
            dprintf(STDOUT_FILENO,"Num of threads must be greater than 1 and smaller than or equal to %ld.\n",nprocs);
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
