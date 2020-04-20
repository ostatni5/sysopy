#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <bits/fcntl-linux.h>
#include <printf.h>
#include <wait.h>
#include <time.h>
#include <sys/file.h>

extern int errno;

int PROCESS_COUNT = 3, COL_PER_P = 1, VERSION = 1;
float MAX_TIME = 100;

void showError(char *msg) {
    printf("---ERROR:  ");
    printf("%s", msg);
    printf("\n");
    exit(-1);
}

float calcTime(clock_t c1, clock_t c2) {
    return ((float) (c2 - c1) / CLOCKS_PER_SEC);
}

int **createMatrix(int col, int row) {
    int **m = calloc(col, sizeof(int *));
    for (int i = 0; i < col; ++i) {
        m[i] = calloc(row, sizeof(int));
    }
    return m;
}

void freeMatrix(int **m, int col, int row) {
    for (int i = 0; i < col; ++i) {
        free(m[i]);
    }
    free(m);
}

typedef struct MatrixInt {
    int **data;
    int col;
    int row;
} MatrixInt;

MatrixInt *createMatrixInt(int col, int row) {
    MatrixInt *m = calloc(1, sizeof(MatrixInt));
    m->col = col;
    m->row = row;
    m->data = createMatrix(col, row);
    return m;
}

void freeMatrixInt(MatrixInt *m) {
    freeMatrix(m->data, m->col, m->row);
    free(m);
}

void  stringToFileMatrixInt(MatrixInt *m, char *name) {

    FILE *file = fopen(name, "w+");
    if (file == NULL)
        perror("fopen fileMatrix");

    int length = m->col * m->row * 5;
    char *line = calloc(length, sizeof(char)), *pos = line;
    sprintf(line, "%i %i\n", m->col, m->row);
    fwrite(line, sizeof(char), strlen(line), file);
    for (int i = 0; i < m->col; ++i) {
        line[0] = '\0';
        pos = line;
        for (int j = 0; j < m->row - 1; ++j) {
            pos += sprintf(pos, "%i ", m->data[i][j]);
        }
        sprintf(pos, "%i\n", m->data[i][m->row - 1]);
        fwrite(line, sizeof(char), strlen(line), file);
    }
    fclose(file);
    free(line);
}

MatrixInt *fileToMatrixInt(char *name) {
    FILE *file = fopen(name, "r");
    if (file == NULL)
        perror("fopen fileMatrix");
    MatrixInt *m;
    char *line = NULL;
    size_t len = 0;
    int col;
    int row;
    if ((getline(&line, &len, file)) != -1) {
        if (sscanf(line, "%i %i", &col, &row) != 2)
            showError("Wrong Matrix");
        if (col <= 0 || row <= 0)
            showError("Wrong Matrix Dims");
        m = createMatrixInt(col, row);
        int i = 0;
        while ((getline(&line, &len, file)) != -1 && i < m->col) {
            int j = 0;
            char *token = strtok(line, " ");
            int num;
            while (token != NULL && j < m->row) {
                if (sscanf(token, "%i", &num) != 1)
                    showError("Not int");
                m->data[i][j] = num;
                j++;
                token = strtok(NULL, " ");
            }
            if (j != m->row)
                showError("Wrong data dim row");
            i++;
        }
        if (i != m->col)
            showError("Wrong data dim col");
        fclose(file);
        free(line);
        return m;
    } else
        showError("Cannot Get");

    fclose(file);
    free(line);
    return NULL;
}

MatrixInt *loadResultMatrixInt(char *name) {
    FILE *file = fopen(name, "r");
    if (file == NULL)
        perror("fopen fileMatrix");
    MatrixInt *m;
    char *line = NULL;
    size_t len = 0;
    int col = 0;
    while ((getline(&line, &len, file)) != -1)
        col++;
    rewind(file);
    int row = -1;
    if ((getline(&line, &len, file)) != -1) {
        char *token = strtok(line, " ");
        while (token != NULL) {
            row++;
            token = strtok(NULL, " ");
        }
        if (col <= 0 || row <= 0)
            showError("Wrong Matrix Dims");
        m = createMatrixInt(col, row);
        rewind(file);
        int i = 0;
        while ((getline(&line, &len, file)) != -1 && i < m->col) {
            int j = 0;
            char *token = strtok(line, " ");
            int colPos;
            if (sscanf(token, "C%i", &colPos) != 1)
                showError("Not int");
            if (colPos < 0 || colPos >= m->col)
                showError("Wrong Pos");

            int num;
            token = strtok(NULL, " ");
            while (token != NULL && j < m->row) {
                if (sscanf(token, "%i", &num) != 1)
                    showError("Not int");
                m->data[colPos][j] = num;
                j++;
                token = strtok(NULL, " ");
            }
            if (j != m->row)
                showError("Wrong data dim row");
            i++;
        }
        if (i != m->col)
            showError("Wrong data dim col");
        fclose(file);
        free(line);
        return m;
    } else
        showError("Cannot Get");

    fclose(file);
    free(line);
    return NULL;
}


void printMatrixInt(MatrixInt *m) {
    for (int i = 0; i < m->col; ++i) {
        for (int j = 0; j < m->row - 1; ++j) {
            printf("%i ", m->data[i][j]);
        }
        printf("%i\n", m->data[i][m->row - 1]);
    }
}

//-------------------------------flocki
int lockFile(int fd) {
    int ret = -1;
    while (1) {
        ret = flock(fd, LOCK_EX | LOCK_NB);
        if (ret == 0) {
            break;
        }
        usleep((rand() % 10) * 1000);
    }
    return ret;
}

int unlockFile(int fd) {
    return flock(fd, LOCK_UN);
}


//-------------------------------normalnie

void saveResult(int id, char ***mNames, int lineNum, MatrixInt *m, int col) {
    char *tempName = calloc(100, sizeof(char));
    if (VERSION == 2)
        sprintf(tempName, "%sT%i", mNames[2][lineNum], id);
    if (VERSION == 1)
        sprintf(tempName, "%s", mNames[2][lineNum]);

    FILE *file = fopen(tempName, "a+");
    if (!file)
        perror("open listFile tempName append");
    int fd = fileno(file);
    int ret = 0;
    if (VERSION == 1) {
        ret = lockFile(fd);
        if (ret != 0)
            showError("Lock errou");
    }

    int length = m->col * m->row * 5;
    char *line = calloc(length, sizeof(char)), *pos;


    pos = line;
    pos += sprintf(line, "C%i ", col);
    for (int j = 0; j < m->row - 1; ++j) {
        pos += sprintf(pos, "%i ", m->data[col][j]);
    }
    sprintf(pos, "%i\n", m->data[col][m->row - 1]);
    fseek(file, 0, SEEK_END);
    fwrite(line, sizeof(char), strlen(line), file);
    if (VERSION == 1) {
        ret = unlockFile(fd);
        if (ret != 0)
            showError("UnLock errou");
    }
    fclose(file);
    free(line);
    free(tempName);
}

void multiplyProcess(int id, char ***mNames, int lineCount) {
    __uint8_t exitCode = 0;
    clock_t cStart = clock(), cEnd;

    for (int i = 0; i < lineCount; ++i) {
        MatrixInt *mA = fileToMatrixInt(mNames[0][i]);
        MatrixInt *mB = fileToMatrixInt(mNames[1][i]);
        MatrixInt *mC = createMatrixInt(mB->col, mA->row);
        char tempName[100];
        if (VERSION == 2) {
            sprintf(tempName, "%sT%i", mNames[2][i], id);
            FILE *file = fopen(tempName, "w+");
            if (!file)
                perror("open listFile tempName Clear");
            fclose(file);
        }
        int maxCol = mB->col;
        for (int col = id * COL_PER_P; col < maxCol;) {
            for (int j = 0; j < COL_PER_P && col < maxCol; j++) {
                printf("1PID %i %i %i\n", (int) getpid(), col, maxCol);
                for (int l = 0; l < mA->row; l++) {
                    int s = 0;
                    for (int k = 0; k < mA->col; k++) s += mA->data[k][l] * mB->data[col][k];
                    mC->data[col][l] = s;
                    exitCode++;
                }
                saveResult(id, mNames, i, mC, col);
                col++;
                cEnd = clock();
                if (calcTime(cStart, cEnd) > MAX_TIME)
                    exit(exitCode);

            }
            col--;
            if (PROCESS_COUNT != 1)
                col += COL_PER_P * PROCESS_COUNT;
            else
                col++;
        }
        freeMatrixInt(mA);
        freeMatrixInt(mB);
        freeMatrixInt(mC);

    }
//    printf("1PID %i mnozenia %i\n", (int) getpid(), exitCode);
    exit(exitCode);
}


void joinFiles(int id) {
    char tempName[100];
    sprintf(tempName, "matrixC%i", id);
    if (VERSION == 2) {
        FILE *tempFile = fopen(tempName, "w+");
        if (!tempFile)
            perror("open listFile");
        for (int i = 0; i < PROCESS_COUNT; i++) {
            int link[2];
            pid_t pid;
            if (pipe(link) == -1)
                showError("pipe");
            if ((pid = fork()) == -1)
                showError("fork");
            char catName[100];
            if (pid == 0) {
                dup2(link[1], STDOUT_FILENO);
                close(link[0]);
                close(link[1]);
                sprintf(catName, "matrixC%iT%i", id, i);
                execlp("cat", "cat", catName, NULL);
            } else {
                wait(&pid);
                close(link[1]);
                char *outputo = calloc(4096, sizeof(char));
                read(link[0], outputo, sizeof(outputo) * 4096);
                fwrite(outputo, sizeof(char), strlen(outputo), tempFile);
                //printf("File C%iT%i:\n%s",id, i,outputo);
                sprintf(catName, "matrixC%iT%i", id, i);
                remove(catName);
                free(outputo);
            }
        }
        fclose(tempFile);
    }
    MatrixInt *m = loadResultMatrixInt(tempName);
    stringToFileMatrixInt(m, tempName);

}

int main(int argc, char **argv) {
    char listFileName[300];
    if (argc == 1)
        printf("No Extra Command Line Argument Passed\n");
    if (argc < 5)
        showError("Not Enough args");
    if (argc >= 5) {
        int argument = 0;
        if (sscanf(argv[++argument], "%s", listFileName) != 1)
            showError("Not String");

        if (sscanf(argv[++argument], "%i", &PROCESS_COUNT) != 1)
            showError("Not int");
        if (PROCESS_COUNT <= 0)
            showError("Negative number");

        if (sscanf(argv[++argument], "%f", &MAX_TIME) != 1)
            showError("Not int");
        if (MAX_TIME < 0)
            showError("Negative number");

        if (sscanf(argv[++argument], "%i", &VERSION) != 1)
            showError("Not int");
        if (VERSION != 1 && VERSION != 2)
            showError("No such version");
    }


    clock_t cStart = clock(), cEnd;
    FILE *listFile = fopen(listFileName, "r");
    if (!listFile)
        perror("open listFile");

    char *line = NULL;
    size_t len = 0;
    int lineCount = 0;
    while ((getline(&line, &len, listFile)) != -1)
        lineCount++;

    char ***mNames = calloc(3, sizeof(char **));
    for (int i = 0; i < 3; ++i) {
        mNames[i] = calloc(lineCount, sizeof(char *));
        for (int j = 0; j < lineCount; ++j) {
            mNames[i][j] = calloc(50, sizeof(char));
        }
    }
    rewind(listFile);
    int counter = 0;
    while ((getline(&line, &len, listFile)) != -1) {
        sscanf(line, "%s %s %s", mNames[0][counter], mNames[1][counter], mNames[2][counter]);
        if (VERSION == 1) {
            FILE *file = fopen(mNames[2][counter], "w+");
            if (!file)
                perror("open matrixC Clear");
            fclose(file);
        }
        counter++;
    }

//    MatrixInt * m1 = fileToMatrixInt(mNames[0][0]);
//    if(!m1)
//        showError("Matrix elooo");
    pid_t *childPid = calloc(PROCESS_COUNT, sizeof(pid_t));

    for (int k = 0; k < PROCESS_COUNT; ++k) {
        childPid[k] = fork();
        if (childPid[k] < 0)
            showError("Fork error");
        if (childPid[k] == 0) {
            multiplyProcess(k, mNames, lineCount);
        }
    }
    for (int k = 0; k < PROCESS_COUNT; ++k) {
        int stat;
        waitpid(childPid[k], &stat, 0);
        printf("Proces %i wykonał %i mnożeń macierzy \n", (int) childPid[k], WEXITSTATUS(stat));
    }


    for (int l = 0; l < lineCount; ++l) {
        joinFiles(l);
    }

    if (line)
        free(line);
    fclose(listFile);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < lineCount; ++j) {
            free(mNames[i][j]);
        }
        free(mNames[i]);
    }
    free(mNames);
    free(childPid);
    cEnd = clock();
    printf("%f\n", calcTime(cStart, cEnd));
    return 0;
}
