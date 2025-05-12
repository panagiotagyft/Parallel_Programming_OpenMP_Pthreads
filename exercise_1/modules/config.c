#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "config.h"

// Function to configure program parameters based on command-line arguments
void config(int argc, char *argv[], long long int *num_darts, int *thread_count)
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
        if (strcmp(argv[1], "-d") > 0)
        {
            printf("Incorrect argument: %s .The correct argument is: -d (num of darts)\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        if (strcmp(argv[3], "-t") > 0)
        {
            printf("Incorrect argument: %s .The correct argument is: -t (threads)\n", argv[3]);
            exit(EXIT_FAILURE);
        }

        if (!isnumber(argv[2]) || !isnumber(argv[4]))
        {
            write(STDOUT_FILENO, "Argument must be positive integer!!\n", 36);
            exit(EXIT_FAILURE);
        }

        // Convert string arguments to integers
        *num_darts = atoi(argv[2]);
        *thread_count = atoi(argv[4]);

        // Check that thread count is greater than 0 if parallel mode is selected
        if (*thread_count == 0)
        {
            write(STDOUT_FILENO, "Fourth argument must be greater than 1.\n", 40);
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
