#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>

typedef struct tms Tms;

float calcTime(clock_t c1, clock_t c2)
{
    return ((float)(c2 - c1) / sysconf(_SC_CLK_TCK));
}

char *timeString(clock_t c1, clock_t c2, Tms *t1, Tms *t2)
{
    char *string = calloc(200, sizeof(char));
    float clock = calcTime(c1, c2);
    float user = calcTime(t1->tms_utime, t2->tms_utime);
    float system = calcTime(t1->tms_stime, t2->tms_stime);
    sprintf(string, "\tClock time:\t %.20f\tUser time:\t %.20f\tSystem time:\t %.20f\t", clock, user, system);
    return string;
}

void reportMsg(char *msg, FILE *file)
{
    //printf("%s", msg);
    fprintf(file, "%s", msg);
}

void reportTime(clock_t c1, clock_t c2, Tms *t1, Tms *t2, FILE *file)
{
    char *string = timeString(c1, c2, t1, t2);
    reportMsg(string, file);
    free(string);
}

void showError(char *msg)
{
    printf("---ERROR:  ");
    printf("%s", msg);
    printf("\n");
    exit(-1);
}

void generate(char *fileName, int count, int length)
{
    char str[250];
    // sprintf(str, "tr -dc A-Za-z0-9 </dev/urandom | head -c %i | fold -w %i > %s", length * count, length, fileName);
    sprintf(str, "tr -dc a-z </dev/urandom | head -c %i | fold -w %i > %s", length * count, length, fileName);
    system(str);
}

char *getLineSys(int fd, int lineNumber, int length)
{
    char *line = calloc(length, sizeof(char));
    lseek(fd, (length + 1) * lineNumber, SEEK_SET);
    read(fd, line, length);
    return line;
}
void setLineSys(int fd, char *line, int lineNumber, int length)
{
    lseek(fd, (length + 1) * lineNumber, SEEK_SET);
    write(fd, line, length);
}

void swapLinesSys(int fd, int lN1, int lN2, int length)
{
    char *line1 = getLineSys(fd, lN1, length);
    char *line2 = getLineSys(fd, lN2, length);
    setLineSys(fd, line1, lN2, length);
    setLineSys(fd, line2, lN1, length);
    free(line1);
    free(line2);
}

int partitionSys(int fd, int low, int high, int length)
{
    char *pivot = getLineSys(fd, high, length);
    char *lineCompared = NULL;
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++)
    {
        lineCompared = getLineSys(fd, j, length);
        if (strcmp(pivot, lineCompared) > 0)
        {
            i++;
            swapLinesSys(fd, i, j, length);
        }
    }
    swapLinesSys(fd, i + 1, high, length);
    free(pivot);
    if (lineCompared != NULL)
        free(lineCompared);
    return (i + 1);
}
void quickSortSys(int fd, int low, int high, int length)
{
    if (low < high)
    {
        int pi = partitionSys(fd, low, high, length);
        quickSortSys(fd, low, pi - 1, length);
        quickSortSys(fd, pi + 1, high, length);
    }
}

void copySys(int fOrgin, int fDestination, int count, int length)
{
    if (count == 0)
        return;
    char *line;
    for (int i = 0; i < count - 1; i++)
    {
        line = getLineSys(fOrgin, i, length);
        write(fDestination, line, length);
        write(fDestination, "\n", 1);
    }
    line = getLineSys(fOrgin, count - 1, length);
    write(fDestination, line, length);
    if (line)
        free(line);
}

//----------------------------LIB----------------------------------

char *getLineLib(FILE *fd, int lineNumber, int length)
{
    char *line = calloc(length, sizeof(char));
    fseek(fd, (length + 1) * lineNumber, 0);
    fread(line, sizeof(char), length, fd);
    return line;
}
void setLineLib(FILE *fd, char *line, int lineNumber, int length)
{
    fseek(fd, (length + 1) * lineNumber, 0);
    fwrite(line, sizeof(char), length, fd);
}

void swapLinesLib(FILE *fd, int lN1, int lN2, int length)
{
    char *line1 = getLineLib(fd, lN1, length);
    char *line2 = getLineLib(fd, lN2, length);
    setLineLib(fd, line1, lN2, length);
    setLineLib(fd, line2, lN1, length);
    free(line1);
    free(line2);
}

int partitionLib(FILE *fd, int low, int high, int length)
{
    char *pivot = getLineLib(fd, high, length);
    char *lineCompared = NULL;
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++)
    {
        lineCompared = getLineLib(fd, j, length);
        if (strcmp(pivot, lineCompared) > 0)
        {
            i++;
            swapLinesLib(fd, i, j, length);
        }
    }
    swapLinesLib(fd, i + 1, high, length);
    free(pivot);
    if (lineCompared != NULL)
        free(lineCompared);
    return (i + 1);
}
void quickSortLib(FILE *fd, int low, int high, int length)
{
    if (low < high)
    {
        int pi = partitionLib(fd, low, high, length);
        quickSortLib(fd, low, pi - 1, length);
        quickSortLib(fd, pi + 1, high, length);
    }
}

void copyLib(FILE *fOrgin, FILE *fDestination, int count, int length)
{

    if (count == 0)
        return;
    char *line;
    for (int i = 0; i < count - 1; i++)
    {
        line = getLineLib(fOrgin, i, length);
        fwrite(line, sizeof(char), length, fDestination);
        fwrite("\n", sizeof(char), 1, fDestination);
    }
    line = getLineLib(fOrgin, count - 1, length);
    fwrite(line, sizeof(char), length, fDestination);
    if (line)
        free(line);
}

int main(int argc, char **argv)
{
    int count = 10, length = 100;

    FILE *fileTimes = fopen("times.txt", "w");
    if (fileTimes == NULL)
        exit(EXIT_FAILURE);

    clock_t cStart, cEnd;
    Tms tStart;
    Tms tEnd;
    printf("Test starts\n");
    if (argc == 1)
        printf("No Extra Command Line Argument Passed\n");
    if (argc >= 3)
    {
        printf("Number Of Arguments Passed: %d\n", argc);
        int argument = 0;
        char *operation = argv[++argument];
        if (strcmp(operation, "generate") == 0)
        {

            if (argc <= argument + 3)
                showError("Not Enough args");

            char fileNameGen[200];
            if (sscanf(argv[++argument], "%s", fileNameGen) != 1)
                showError("Not String");

            if (sscanf(argv[++argument], "%i", &count) != 1)
                showError("Not int");
            if (count < 0)
                showError("Negative number");

            if (sscanf(argv[++argument], "%i", &length) != 1)
                showError("Not int");
            if (length < 0)
                showError("Negative number");
            cStart = times(&tStart);
            generate(fileNameGen, count, length);
            cEnd = times(&tEnd);
            reportMsg("Time of generating ", fileTimes);
            reportTime(cStart, cEnd, &tStart, &tEnd, fileTimes);
            reportMsg("\n", fileTimes);
        }
        else if (strcmp(operation, "sort") == 0)
        {
            if (argc <= argument + 4)
                showError("Not Enough args");

            char fileNameGen[200];
            if (sscanf(argv[++argument], "%s", fileNameGen) != 1)
                showError("Not String");

            if (sscanf(argv[++argument], "%i", &count) != 1)
                showError("Not int");
            if (count < 0)
                showError("Negative number");

            if (sscanf(argv[++argument], "%i", &length) != 1)
                showError("Not int");
            if (length < 0)
                showError("Negative number");

            char mode[20];
            if (sscanf(argv[++argument], "%s", mode) != 1)
                showError("Not String");
            if (strcmp(mode, "lib") == 0)
            {
                cStart = times(&tStart);
                FILE *sort = fopen(fileNameGen, "r+");
                if (sort == NULL)
                    showError("File for sorting not opened");
                quickSortLib(sort, 0, count - 1, length);
                cEnd = times(&tEnd);
                reportMsg("Time of sorting lib", fileTimes);
                reportTime(cStart, cEnd, &tStart, &tEnd, fileTimes);
                reportMsg("\n", fileTimes);
            }
            else if (strcmp(mode, "sys") == 0)
            {
                cStart = times(&tStart);
                int sort = open(fileNameGen, O_RDWR);
                if (sort < 0)
                    showError("File for sorting not opened");
                quickSortSys(sort, 0, count - 1, length);
                cEnd = times(&tEnd);
                reportMsg("Time of sorting sys", fileTimes);
                reportTime(cStart, cEnd, &tStart, &tEnd, fileTimes);
                reportMsg("\n", fileTimes);
            }
            else
                showError("Invalid mode");
        }
        else if (strcmp(operation, "copy") == 0)
        {
            if (argc <= argument + 4)
                showError("Not Enough args");

            char fileNameOrgin[200];
            if (sscanf(argv[++argument], "%s", fileNameOrgin) != 1)
                showError("Not String");

            char fileNameDestination[200];
            if (sscanf(argv[++argument], "%s", fileNameDestination) != 1)
                showError("Not String");

            if (sscanf(argv[++argument], "%i", &count) != 1)
                showError("Not int");
            if (count < 0)
                showError("Negative number");

            if (sscanf(argv[++argument], "%i", &length) != 1)
                showError("Not int");
            if (length < 0)
                showError("Negative number");

            char mode[20];
            if (sscanf(argv[++argument], "%s", mode) != 1)
                showError("Not String");
            if (strcmp(mode, "sys") == 0)
            {
                cStart = times(&tStart);
                int fileOrgin = open(fileNameOrgin, O_RDWR);
                if (fileOrgin < 0)
                    showError("File for copy  [sys]");
                int fileDestination = open(fileNameDestination, O_WRONLY | O_CREAT | O_TRUNC);
                if (fileDestination < 0)
                    showError("File for paste  [sys]");
                copySys(fileOrgin, fileDestination, count, length);
                cEnd = times(&tEnd);
                reportMsg("Time of copying sys", fileTimes);
                reportTime(cStart, cEnd, &tStart, &tEnd, fileTimes);
                reportMsg("\n", fileTimes);
            }
            else if (strcmp(mode, "lib") == 0)
            {
                cStart = times(&tStart);
                FILE *fileOrgin = fopen(fileNameOrgin, "r");
                if (fileOrgin == NULL)
                    showError("File for copy  [lib]");
                FILE *fileDestination = fopen(fileNameDestination, "w");
                if (fileDestination == NULL)
                    showError("File for paste [lib]");
                copyLib(fileOrgin, fileDestination, count, length);
                cEnd = times(&tEnd);
                reportMsg("Time of copying lib", fileTimes);
                reportTime(cStart, cEnd, &tStart, &tEnd, fileTimes);
                reportMsg("\n", fileTimes);
            }
            else
                showError("Invalid mode");
        }
        else
            showError("Invalid operation");
    }

    fclose(fileTimes);

    return 0;
}