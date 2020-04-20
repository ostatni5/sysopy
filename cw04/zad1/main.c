#include <stdio.h>
#include <signal.h>
#include <bits/signum.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

bool flagTSTP=false;
void handleINT(int signum)
{
    printf("\nOdebrano sygnał SIGINT\n");
    exit(0);
}

void handleTSTP(int sig_no) {
    printf( "\nCTRL+Z\n");
    if(!flagTSTP)
        printf( "\nOczekuję na CTRL+Z - kontynuacja albo CTR+C - zakończenie programu\n");
    flagTSTP=!flagTSTP;
}


int main() {
    signal(SIGINT, handleINT);
    struct sigaction act;
    act.sa_handler = handleTSTP;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGTSTP, &act, NULL);
    while(1)
    {
        if(!flagTSTP)
            system("ls");
        sleep(1);
    }

    return 0;
}
