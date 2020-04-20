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

int flagsValues[3];       //mtime, atime, maxdepth;
int flagsSign[] = {0, 0}; //mtime, atime, maxdepth;
bool flags[] = {false, false, false};
char *FILTR;

void showError(char *msg)
{
    printf("---ERROR:  ");
    printf("%s", msg);
    printf("\n");
    exit(-1);
}

char *absolutePath(char *path, char *pathCall)
{
    char *actualpath = calloc(PATH_MAX, sizeof(char));
    char *ptrPath = strdup(pathCall);
    char *ptr;
    char *subPath = calloc(PATH_MAX, sizeof(char));
    strcat(subPath, ptrPath);
    strcat(subPath, "/");
    strcat(subPath, path);
    ptr = realpath(subPath, actualpath);
    pathCall = strdup(ptrPath);
    free(ptrPath);
    free(subPath);
    return ptr;
}

float dayDiff(time_t time1, time_t time2)
{
    return difftime(time1, time2) / 60 / 60 / 24;
}

bool bettwenF(float f1, float f2, float value)
{
    return (value <= f2 && value >= f1);
}

bool checkFlagTimeRange(int flag, float day)
{
    if (flags[flag])
    {
        if (flagsSign[flag] == 0)
            return bettwenF(flagsValues[flag], flagsValues[flag] + 1, day);
        else if (flagsSign[flag] == 1)
            return (day >= (float)flagsValues[flag]);
        else
            return (day <= (float)flagsValues[flag]);
    }
    else
        return true;
}

bool printFileStat(struct dirent *dp, char *pathCall)
{
    bool directory = false;
    struct stat sb;
    char *abs = absolutePath(dp->d_name, pathCall);
    stat(abs, &sb);
    time_t timeNow = time(NULL);
    float dayModify = dayDiff(timeNow, sb.st_mtime);
    float dayAccess = dayDiff(timeNow, sb.st_atime);
    if (!checkFlagTimeRange(0, dayModify) || !checkFlagTimeRange(1, dayAccess))
    {
        free(abs);
        return false;
    }

    printf("---------------------------\n");
    printf("File name:                %s\n", dp->d_name);
    printf("File path:                %s\n", abs);
    printf("File type:                ");

    switch (sb.st_mode & S_IFMT)
    {
    case S_IFBLK:
        printf("block device\n");
        break;
    case S_IFCHR:
        printf("character device\n");
        break;
    case S_IFDIR:
        printf("directory\n");
        directory = true;
        break;
    case S_IFIFO:
        printf("FIFO/pipe\n");
        break;
    case S_IFLNK:
        printf("symlink\n");
        break;
    case S_IFREG:
        printf("regular file\n");
        break;
    case S_IFSOCK:
        printf("socket\n");
        break;
    default:
        printf("unknown?\n");

        break;
    }
    printf("Link count:               %ld\n", (long)sb.st_nlink);

    printf("File size:                %lld bytes\n",
           (long long)sb.st_size);
    printf("Last status change:       %s", ctime(&sb.st_ctime));
    printf("Last file access:         %s", ctime(&sb.st_atime));
    printf("Last file modification:   %s", ctime(&sb.st_mtime));
    // printf("dayAccess:   %f\n", dayAccess);
    // printf("dayModify:   %f\n", dayModify);
    free(abs);
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
        return false;

    return directory;
}
bool isDir(struct dirent *dp, char *pathCall)
{
    struct stat sb;
    char *abs = absolutePath(dp->d_name, pathCall);
    stat(abs, &sb);
    free(abs);
    return ((sb.st_mode & S_IFMT) == S_IFDIR);
}

void findo(char *dirname, char *name, int depth)
{
    //printf("----->open %s\n", dirname);
    if (flags[2] && depth > flagsValues[2])
        return;

    DIR *dirp = opendir(dirname);
    if (!dirp)
    {
        printf("Cant open %s\n", dirname);
        return;
    }
    struct dirent *dp = readdir(dirp);

    while (dp != NULL)
    {
        if (strcmp(name, "*") == 0 || strstr(dp->d_name, name) != NULL)
        {
            if (!(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0))

                if (printFileStat(dp, dirname))
                {
                    char *newPatho = absolutePath(dp->d_name, dirname);
                    findo(newPatho, name, (depth + 1));
                    free(newPatho);
                }
        }
        else if (isDir(dp, dirname))
        {
            char *newPatho = absolutePath(dp->d_name, dirname);
            if (!(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0))
                findo(newPatho, name, (depth + 1));
            free(newPatho);
        }

        dp = readdir(dirp);
    }

    if (dirp)
        closedir(dirp);
}

int displayInfo(const char *fpath, const struct stat *sb,
                 int tflag, struct FTW *ftwbuf)
{
    if (!(strcmp(FILTR, "*") == 0) && !(strstr(fpath + ftwbuf->base, FILTR) != NULL))
        return 0;

    if (flags[2] && ftwbuf->level - 1 > flagsValues[2])
    {
        return 0;
    }
    char *abs = calloc(PATH_MAX, sizeof(char));
    realpath(fpath, abs);
    time_t timeNow = time(NULL);
    float dayModify = dayDiff(timeNow, sb->st_mtime);
    float dayAccess = dayDiff(timeNow, sb->st_atime);
    if (!checkFlagTimeRange(0, dayModify) || !checkFlagTimeRange(1, dayAccess))
    {
        free(abs);
        return 0;
    }

    printf("---------------------------\n");
    printf("File name:                %s\n", fpath + ftwbuf->base);
    printf("File path:                %s\n", abs);
    printf("File type:                ");

    switch (sb->st_mode & S_IFMT)
    {
    case S_IFBLK:
        printf("block device\n");
        break;
    case S_IFCHR:
        printf("character device\n");
        break;
    case S_IFDIR:
        printf("directory\n");
        break;
    case S_IFIFO:
        printf("FIFO/pipe\n");
        break;
    case S_IFLNK:
        printf("symlink\n");
        break;
    case S_IFREG:
        printf("regular file\n");
        break;
    case S_IFSOCK:
        printf("socket\n");
        break;
    default:
        printf("unknown?\n");

        break;
    }
    printf("Link count:               %ld\n", (long)sb->st_nlink);

    printf("File size:                %lld bytes\n",
           (long long)sb->st_size);
    printf("Last status change:       %s", ctime(&(*sb).st_ctime));
    printf("Last file access:         %s", ctime(&(*sb).st_atime));
    printf("Last file modification:   %s", ctime(&(*sb).st_mtime));

    return 0; /* To tell nftw() to continue */
}

int main(int argc, char **argv)
{

    char *dirname = calloc(PATH_MAX, sizeof(char)), *name = calloc(PATH_MAX, sizeof(char));
    FILTR = name;

    if (argc == 1)
        printf("No Extra Command Line Argument Passed\n");
    if (argc < 3)
        showError("Not Enough args");
    if (argc >= 3)
    {
        int argument = 0;
        if (sscanf(argv[++argument], "%s", dirname) != 1)
            showError("Not String");
        if (sscanf(argv[++argument], "%s", name) != 1)
            showError("Not String");
        while (argument + 1 < argc)
        {
            char *flag = argv[++argument];
            if (argc <= argument + 1)
                showError("Not Enough args");

            if (strcmp(flag, "-mtime") == 0)
            {
                char *num = argv[++argument];
                if (num[0] == '-')
                    flagsSign[0] = -1;
                else if (num[0] == '+')
                    flagsSign[0] = 1;

                if (sscanf(num, "%i", &flagsValues[0]) != 1)
                    showError("Not int");
                flagsValues[0] = abs(flagsValues[0]);

                flags[0] = true;
            }
            else if (strcmp(flag, "-atime") == 0)
            {
                char *num = argv[++argument];
                if (num[0] == '-')
                    flagsSign[1] = -1;
                else if (num[0] == '+')
                    flagsSign[1] = 1;

                if (sscanf(num, "%i", &flagsValues[1]) != 1)
                    showError("Not int");
                flagsValues[1] = abs(flagsValues[1]);

                flags[1] = true;
            }
            else if (strcmp(flag, "-maxdepth") == 0)
            {
                if (sscanf(argv[++argument], "%i", &flagsValues[2]) != 1)
                    showError("Not int");
                if (flagsValues[2] < 0)
                    showError("Negative number");
                flags[2] = true;
            }
        }
        //option1 ------- Korzystając z funkcji opendir(), readdir() oraz funkcji z rodziny stat (25%)
        findo(dirname, name, 0);
        //option2 ------- Korzystając z funkcji nftw() (20%)
        //nftw(dirname, displayInfo, 20, FTW_PHYS);
        free(dirname);
        free(name);
    }
    return 0;
}
