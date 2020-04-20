#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char FILENAME[100], FIFONAME[100];
int N;

void showError(char *msg)
{
    perror(msg);
    printf("---ERROR:  ");
    printf("%s", msg);
    printf("\n");
    exit(-1);
}

int main(int argc, char **argv)
{
    if (argc == 4)
    {
        printf("Producent\n");
        int argument = 0;
        if (sscanf(argv[++argument], "%s", FIFONAME) != 1)
            showError("Not string");
        if (sscanf(argv[++argument], "%s", FILENAME) != 1)
            showError("Not string");
        if (sscanf(argv[++argument], "%i", &N) != 1)
            showError("Not int");
        if (N <= 0)
            showError("No such Variant");

        FILE *fileSource = fopen(FILENAME, "r");
        if (!fileSource)
            showError("fileSource");

        FILE *fileFifo = fopen(FIFONAME, "w");
        if (!fileFifo)
            showError("fileFifo");

        char *buff = calloc(N, sizeof(char));
        int len = 0;
        while ((len = fread(buff, sizeof(char), N, fileSource)) > 0)
        {            
            fprintf(fileFifo, "#%i#", getpid());
            fwrite(buff, sizeof(char), len, fileFifo);
            fprintf(fileFifo, "\n");
            sleep(1+rand()%2);
        }

        free(buff);
        fclose(fileFifo);
        fclose(fileSource);
    }
    else
    {
        printf("Wrong number of args");
    }

    return 0;
}
