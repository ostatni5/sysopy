#include <stdio.h>
#include <signal.h>
#include <bits/signum.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wait.h>


typedef enum Variant {
    IGNORE, HANDLER, MASK, PENDING
} Variant;

Variant VARIANT;

void showError(char *msg) {
    printf("---ERROR:  ");
    printf("%s", msg);
    printf("\n");
    exit(-1);
}
void handleUSR1default(int sig_no) {
    printf("\nSIGUSR1 DEFAULT EXEC\n");
}


int main(int argc, char **argv) {
    printf("child\n");
    if (argc == 1)
        printf("No Extra Command Line Argument Passed\n");

    if (argc >= 2) {
        int argument = 0;
        int number;
        if (sscanf(argv[++argument], "%i", &number) != 1)
            showError("Not int");
        if (number < 0 || number > 3)
            showError("No such Variant");
        VARIANT = number;

        struct sigaction act;
        act.sa_handler = handleUSR1default;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        sigaction(SIGUSR1, &act, NULL);



        if (VARIANT == IGNORE) {
            raise(SIGUSR1);
        }  else if (VARIANT == MASK) {
            raise(SIGUSR1);
        } else if (VARIANT == PENDING) {
            sigset_t pendingSet;
            sigpending(&pendingSet);
            if (sigismember(&pendingSet, SIGUSR1) == 1)
                printf("SIGUSR1: oczekuje\n");
            else
                printf("SIGUSR1: nie oczekuje\n");
        }

    }

    return 0;
}