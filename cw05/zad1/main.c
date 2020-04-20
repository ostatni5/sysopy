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
        printf("No Extra Command Line Argument Passed\n");

    if (argc >= 3) {
        printf("Start\n");
        int argument = 0;
        if (sscanf(argv[++argument], "%s", FILENAME) != 1)
            showError("Not string");

        int number;
        if (sscanf(argv[++argument], "%i", &number) != 1)
            showError("Not int");
        if (number < 0 || number > 1)
            showError("No such Variant");
        VERSION = number;


        FILE *cFile = fopen(FILENAME, "r");
        if (!cFile)
            perror("open cFile");
        char *line = NULL;
        size_t len = 0;


        while ((getline(&line, &len, cFile)) != -1) {
            printf("LINE %s\n", line);
            int *FDS[50];

            for (int k = 0; k < 50; ++k) {
                FDS[k] = calloc(2, sizeof(int));
            }
            pipe(FDS[0]);
            char *program = strtok(line, "|");
            int programCount = 1;
            while (program != NULL) {
                printf("PROGHRAM %s\n", program);
                char *arg = NULL;
                int countArgs = 0;
                char **argTab = calloc(6, sizeof(char *));
                for (int i = 0; i < 6; i++)
                    argTab[i] = NULL;
                while ((arg = strsep(&program, " ")) && countArgs < 6) {
                    if (arg[0] != '\0') {
                        argTab[countArgs++] = strdup(arg);
                    }
                }

                pipe(FDS[programCount]);
                pid_t pid = fork();
                if (pid == 0) {
                    fclose(cFile);
                    close(FDS[programCount - 1][1]);
                    close(FDS[programCount][0]);
                    dup2(FDS[programCount - 1][0], STDIN_FILENO);
                    dup2(FDS[programCount][1], STDOUT_FILENO);
                    if (argTab[0][0] == '.')
                        execv(argTab[0], argTab);
                    execvp(argTab[0], argTab);
                    perror("execv");
                } else {
                    close(FDS[programCount - 1][0]);
                    close(FDS[programCount - 1][1]);
                    wait(NULL);

                }


                for (int i = 0; i < countArgs; i++)
                    free(argTab[i]);
                free(argTab);
                free(arg);
                program = strtok(NULL, "|");
                programCount++;
            }

            close(FDS[programCount - 1][0]);
            close(FDS[programCount - 1][1]);
            free(program);
            for (int k = 0; k < 50; ++k) {
                free(FDS[k]);
            }
            for (int j = 1; j < programCount; ++j) {

            }

        }

        fclose(cFile);
        free(line);


    }
    return 0;
}
