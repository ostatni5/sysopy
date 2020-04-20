#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

int main()
{
    printf("Main:STAR...\n");

    if (mkfifo("/tmp/tempFile", 0666) == -1 && errno != EEXIST)
    {
        perror("mkfifo");
        exit(-1);
    }
    perror("mkfifo");
    
    for (int i = 1; i <= 5; i++)
    {
        char name[30];
        sprintf(name, "source%i", i);
        char command[200];
        sprintf(command, "./producent /tmp/tempFile %s 4", name);
        pid_t pid = fork();
        if (pid == 0)
        {
            printf("%s\n", command);
            char *const av[] = {"producent", "/tmp/tempFile", name,"4",NULL};
            execv("./producent", av);
            exit(-1);
        }
    }
    pid_t pid = fork();
    if (pid == 0)
    {
        printf("./client /tmp/tempFile dest 4\n");
        char *const av[] = {"client", "/tmp/tempFile", "dest","4",NULL};
        execv("./client", av);
        exit(-1);
    }

    for (int i = 1; i <= 6; i++)
        wait(NULL);

    printf("Main:END...\n");
    return 0;
}
