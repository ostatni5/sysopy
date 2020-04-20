#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <stdlib.h>


void handleUSR1(int sig, siginfo_t *info, void *ucontext) {
    printf("\nSIGUSR PID:%i\n", getpid());
    printf("Signal code:%i\n", info->si_code);
    printf("Real user ID of sending process:%i\n", info->si_uid);
    printf("Exit value or signal:%i\n", info->si_status);
    printf("File descriptor:%i\n", info->si_fd);
    printf("User time consumed:%li\n", info->si_utime);
    printf("System time consumed:%li\n", info->si_stime);
    printf("Signal value:%i\n", info->si_value.sival_int);

}


int main() {
    struct sigaction act;
    act.sa_sigaction = handleUSR1;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO ;
    sigaction(SIGUSR1, &act, NULL);

    raise(SIGUSR1);

    struct sigaction act2;
    act2.sa_sigaction = handleUSR1;
    sigemptyset(&act2.sa_mask);
    act2.sa_flags = SA_SIGINFO ;
    sigaction(SIGUSR2, &act, NULL);
    raise(SIGUSR2);

    pid_t child_pid = fork();
    if (child_pid < 0)
       exit(0);
    if (child_pid == 0)
    {
        raise(SIGUSR1);
        raise(SIGUSR2);
        exit(0);
    }
    else
    {
        wait(&child_pid);
    }

    return 0;
}
