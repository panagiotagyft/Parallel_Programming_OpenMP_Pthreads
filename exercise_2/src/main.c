#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include "mutual_exclution.h"
#include "atomic_operations.h"

int flag = 0;

void handle_sigint(int sig)
{
    (void)sig;
    const char *msg = "\n[!] Ctrl+C pressed. Cleaning up...\n";
    write(STDOUT_FILENO, msg, strlen(msg));

    handle_sigint_mutex();  // free + destroy mutex
    if(flag == 1)
        handle_sigint_atomic(); // free threads

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

    // printf(">>> Starting mutual_exclution...\n");
    mutual_exclution(thread_count, iterations);
    // printf(">>> Starting atomic_operations...\n");
    flag = 1;
    atomic_operations(thread_count, iterations);

    return EXIT_SUCCESS;
}
