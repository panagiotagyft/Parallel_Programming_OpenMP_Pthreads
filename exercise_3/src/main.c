#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include "work_array.h"

int *array = NULL;

void handle_sigint(int sig)
{
    (void)sig;
    const char *msg = "\n[!] Ctrl+C pressed. Cleaning up...\n";
    write(STDOUT_FILENO, msg, strlen(msg));

    handle_sigint_array();  // free + destroy mutex
    
    if(array != NULL)
        free(array);

    _exit(EXIT_SUCCESS); // άμεσος τερματισμός
}


int main(int argc, char *argv[])
{
    int thread_count, iterations;
    struct sigaction sigint_act;
    
    // Signal Handler for SIGINT
    memset(&sigint_act, 0, sizeof(sigint_act));
    sigint_act.sa_handler = handle_sigint;
    if (sigaction(SIGINT, &sigint_act, NULL) == -1)
    {
        perror("ERROR: sigaction() failed\n");
        exit(EXIT_FAILURE);
    }

    config(argc, argv, &iterations, &thread_count);

    array = calloc(thread_count, sizeof(int));
    if (array == NULL)
    {
        write(STDERR_FILENO, "ERROR Not available memory\n", 27);
        return EXIT_FAILURE;
    }

    // printf(">>> Starting mutual_exclution...\n");
    work_array(thread_count, iterations, array, false);
    int sum = 0;
    for (int i=0; i < thread_count; i++){
        sum += array[i];
    }
    // printf("Sum: %d\n", sum);
    free(array);

    array = calloc(thread_count, sizeof(int));
    if (array == NULL)
    {
        write(STDERR_FILENO, "ERROR Not available memory\n", 27);
        return EXIT_FAILURE;
    }
    // printf(">>> Starting mutual_exclution...\n");
    work_array(thread_count, iterations, array, true);
    sum = 0;
    for (int i=0; i < thread_count; i++)
    {
        sum += array[i];
    }
    // printf("Sum: %d\n", sum);

    free(array);

    return EXIT_SUCCESS;
}
