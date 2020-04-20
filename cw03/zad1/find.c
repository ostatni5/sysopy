// -mtime n
//       Dane pliku były ostatnio modyfikowane n*24 godzin temu.
// -atime n
//       Do pliku dostano się po raz ostatni n*24 godzin temu.
// -maxdepth głębokość
//     Schodzi   maksymalnie  głębokość  (nieujemna  liczba  całkowita)
//     poziomów katalogów od argumentów wiersza poleceń. `-maxdepth  0'
//     oznacza,  by  testy i akcje stosować tylko do argumentów wiersza
//     poleceń.

#define _XOPEN_SOURCE 1          /* Required under GLIBC for nftw() */
#define _XOPEN_SOURCE_EXTENDED 1 /* Same */

#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <getopt.h>
#include <ftw.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t PARENT_PID;

void showError(char *msg)
{
    printf("---ERROR:  ");
    printf("%s", msg);
    printf("\n");
    exit(-1);
}

int displayInfo(const char *fpath, const struct stat *sb,
                int tflag, struct FTW *ftwbuf)
{

    if (tflag == FTW_D)
    {
        pid_t child_pid = fork();
        if (child_pid < 0)
            showError("Fork error");
        if (child_pid == 0)
        {
            printf("---------------------------\n");
            printf("PID:       %i\n", (int)getpid());
            printf("Relaiive path:                %s\n", fpath);
            char command[100] = "ls -l ";
            strcat(command, fpath);
            system(command);
            exit(0);
        }
        else
        {
            wait(&child_pid);
        }
        
    }

    return 0; /* To tell nftw() to continue */
}

int main(int argc, char **argv)
{

    PARENT_PID = getpid();
    char *dirname = calloc(PATH_MAX, sizeof(char)), *name = calloc(PATH_MAX, sizeof(char));

    if (argc == 1)
        printf("No Extra Command Line Argument Passed\n");
    if (argc < 2)
        showError("Not Enough args");
    if (argc >= 2)
    {
        int argument = 0;
        if (sscanf(argv[++argument], "%s", dirname) != 1)
            showError("Not String");

        nftw(dirname, displayInfo, 20, FTW_PHYS);
        free(dirname);
        free(name);
    }
    return 0;
}
