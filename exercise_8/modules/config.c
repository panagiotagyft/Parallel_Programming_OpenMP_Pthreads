#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "config.h"

// Function to configure program parameters based on command-line arguments
void config(int argc, char *argv[], long long int *array_size, int *parallel_impl, int *thread_count)
{
    if (argc < 4)
    {
        write(STDOUT_FILENO, "Missing arguments...\n", 21);
        exit(EXIT_FAILURE);
    }
    else if (argc > 4)
    {
        write(STDOUT_FILENO, "Too many arguments...\n", 22);
        exit(EXIT_FAILURE);
    }
    else
    {
        if (!isnumber(argv[1]) || !isnumber(argv[2]) || !isnumber(argv[3]))
        {
            write(STDOUT_FILENO, "Arguments must be positive integers!!\n", 38);
            exit(EXIT_FAILURE);
        }

        // Convert string arguments to integers
        *array_size = atoi(argv[1]);
        *parallel_impl = atoi(argv[2]);
        *thread_count = atoi(argv[3]);

        // Check for valid parallel implementation value
        if (*parallel_impl != 0 && *parallel_impl != 1)
        {
            write(STDOUT_FILENO, "Second argument must be 0 (serial) or 1 (parallel).\n", 52);
            exit(EXIT_FAILURE);
        }

        // Check that thread count is greater than 0 if parallel mode is selected
        if (*parallel_impl == 1 && *thread_count == 0)
        {
            write(STDOUT_FILENO, "Fourth argument must be greater than 1.\n", 42);
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
