#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

int VERSION = 0;
char FILENAME[100];

void showError(char *msg) {
    printf("---ERROR:  ");
    printf("%s", msg);
    printf("\n");
    exit(-1);
}


int main(int argc, char **argv) {
    if (argc == 1)
        fprintf(stdout, "No Extra Command Line Argument Passed ");
    int argument = 0;
    FILE *cFile = fopen("FILENAME", "a");
    if (!cFile)
        perror("open cFile");
    while (++argument < argc) {
        fprintf(stdout, "%s ", argv[argument]);
        fprintf(cFile, "%s ", argv[argument]);
    }
    if (argc < 2  || argv[1][0] != 'a')
       {
            char str[4096];
            while(fgets(str, 4096, stdin) )
           {
               fprintf(stdout, "%s", str);
               fprintf(cFile, "%s", str);
           }
        }
    printf("\n");
    fprintf(cFile, "\n");
    fclose(cFile);
    return 0;
}
