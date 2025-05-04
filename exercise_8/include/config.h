#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdbool.h>

// Function to configure program parameters based on command-line arguments
void config(int, char **, long long int *, int *, int *);

// Helper function to check if a string is a number (contains only digits)
bool isnumber(const char *);

#endif // CONFIG_H