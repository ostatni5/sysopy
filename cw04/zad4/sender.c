#include <stdio.h>
#include <unistd.h>
#include <bits/types/siginfo_t.h>
#include <signal.h>
#include <stdlib.h>
int VERSION=0;
int COUNT_SIG1=0,AMOUNT,PID;
void showError(char *msg) {
    printf("---ERROR:  ");
    printf("%s", msg);
    printf("\n");
    exit(-1);
}
void handleUSR1(int sig, siginfo_t *info, void *ucontext) {
    printf("SIGUSR1 %i",++COUNT_SIG1);
    if(VERSION==1)
    {
        printf(" %i",info->si_value.sival_int);
    }
    printf("\n");

}
void handleUSR2(int sig, siginfo_t *info, void *ucontext) {
    printf("SIGUSR2\n");
    printf("Odebral:  %i\n",COUNT_SIG1);
    printf("Powinien: %i\n",AMOUNT);
    exit(0);

}

void sendSIG1(int n)
{
    printf("SEND 1\n");
    if(VERSION==0)
    {
        kill(PID,SIGUSR1);
    }else if(VERSION==1)
    {
        union sigval value;
        value.sival_int=n;
        if(sigqueue(PID, SIGUSR1 ,value) != 0) {
            perror("SIGSENT-ERROR:");
            kill(PID,SIGUSR2);
            exit(0);
        }

    }
    else
    if(VERSION==2)
    {
        kill(PID,SIGRTMIN+1);
    }

}
void sendSIG2()
{
    printf("SEND 2\n");
    if(VERSION==0)
    {
        kill(PID,SIGUSR2);
    } else
    if(VERSION==1)
    {
        union sigval value;
        if(sigqueue(PID, SIGUSR2, value) != 0) {
            perror("SIGSENT-ERROR:");
        }
    } else
    if(VERSION==2)
    {
        kill(PID,SIGRTMIN+2);
    }
}

int main(int argc, char **argv) {
    if (argc == 1)
        printf("No Extra Command Line Argument Passed\n");

    if (argc >= 2) {
        int argument = 0;
        int number;
        if (sscanf(argv[++argument], "%i", &number) != 1)
            showError("Not int");
        if (number < 0 || number > 2)
            showError("No such Variant");
        VERSION = number;

        if (sscanf(argv[++argument], "%i", &number) != 1)
            showError("Not int");
        if (number < 0 )
            showError("No such amount");
        AMOUNT = number;

        if (sscanf(argv[++argument], "%i", &number) != 1)
            showError("Not int");
        if (number <= 0 )
            showError("No such PID");
        PID = number;

        sigset_t newmask;
        sigfillset (&newmask);
        sigdelset  (&newmask,SIGUSR1);
        sigdelset  (&newmask,SIGUSR2);
        if(VERSION==2)
        {
            sigfillset (&newmask);
            sigdelset  (&newmask,SIGRTMIN+1);
            sigdelset  (&newmask,SIGRTMIN+2);
        }

        if (sigprocmask(SIG_BLOCK, &newmask, NULL) < 0)
            perror("Nie udało się zablokować sygnału");
        struct sigaction act;
        act.sa_sigaction = handleUSR1;
        sigemptyset(&act.sa_mask);
        act.sa_flags = SA_SIGINFO ;
        sigaction(SIGUSR1, &act, NULL);

        struct sigaction act2;
        act2.sa_sigaction = handleUSR2;
        sigemptyset(&act2.sa_mask);
        act2.sa_flags = SA_SIGINFO ;
        sigaction(SIGUSR2, &act2, NULL);
        if(VERSION==2)
        {
            sigaction(SIGRTMIN+1, &act, NULL);
            sigaction(SIGRTMIN+2, &act2, NULL);
        }

        for (int i = 0; i <AMOUNT ; ++i) {
            sendSIG1(i);
        }
        sendSIG2();

        while (1)
            sleep(1);


    }


    return 0;
}