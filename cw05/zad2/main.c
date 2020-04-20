#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
char FILENAME[100];
int main(int argc, char **argv)
{
    if (argc == 1)
        printf("No Extra Command Line Argument Passed\n");

    if (argc >= 2)
    {
        printf("Start\n");
        int argument = 0;
        if (sscanf(argv[++argument], "%s", FILENAME) != 1)
           exit(-1);

        char str[100];
        strcpy(str, "sort ");
        strcat(str, FILENAME);

        FILE *filum = popen(str, "w");
        perror("popen");
        pclose(filum);
    }
    return 0;
}
