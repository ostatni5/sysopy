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
    printf("\nSIGUSR1 DEFAULT\n");
}

void handleUSR1(int sig_no) {
    printf("\nSIGUSR1 PID:%i\n", getpid());
}

void maskSIG()
{
    sigset_t newmask;
    sigset_t oldmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        perror("Nie udało się zablokować sygnału");
    if (sigprocmask(SIG_SETMASK, &newmask, NULL) < 0)
        perror("Nie udało się przywrócić maski sygnałów");
}

void installHandle()
{
    struct sigaction act;
    act.sa_handler = handleUSR1;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);
}

int main(int argc, char **argv) {
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
            signal(SIGUSR1, SIG_IGN);
            raise(SIGUSR1);
        } else if (VARIANT == HANDLER) {
            installHandle();
            raise(SIGUSR1);
        } else if (VARIANT == MASK) {
            maskSIG();
            raise(SIGUSR1);
        } else if (VARIANT == PENDING) {
            maskSIG();
            raise(SIGUSR1);
            sigset_t pendingSet;
            sigpending(&pendingSet);
            if (sigismember(&pendingSet, SIGUSR1) == 1)
                printf("SIGUSR1: oczekuje\n");
            else
                printf("SIGUSR1: nie oczekuje\n");
        }

        pid_t child_pid = fork();
        if (child_pid < 0)
            showError("Fork error");
        if (child_pid == 0)
        {
            printf("POTOMEK HERE\n");
            if (VARIANT == IGNORE) {
                raise(SIGUSR1);
            } else if (VARIANT == HANDLER) {
                raise(SIGUSR1);
            } else if (VARIANT == MASK) {
                raise(SIGUSR1);
            } else if (VARIANT == PENDING) {
                sigset_t pendingSet;
                sigpending(&pendingSet);
                if (sigismember(&pendingSet, SIGUSR1) == 1)
                    printf("SIGUSR1: oczekuje\n");
                else
                    printf("SIGUSR1: nie oczekuje\n");
            }
            exit(0);
        }
        else
        {
            wait(&child_pid);
        }

        printf("EXEC HERE\n");
        if (VARIANT == IGNORE) {
            execl("./child", "child", "0",NULL);
        } else if (VARIANT == MASK) {
            execl("./child", "child", "2",NULL);
        } else if (VARIANT == PENDING) {
            execl("./child", "child", "3",NULL);
        }




    }

    return 0;
}