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





void multiplyProcess(char ***mNames, int lineCount) {
    for (int i = 0; i < lineCount; ++i) {
        printf("%s %s %s is:", mNames[0][i], mNames[1][i], mNames[2][i]);
        MatrixInt *mA = fileToMatrixInt(mNames[0][i]);
        MatrixInt *mB = fileToMatrixInt(mNames[1][i]);
        MatrixInt *mCTrue = fileToMatrixInt(mNames[2][i]);
        MatrixInt *mC = createMatrixInt(mB->col, mA->row);
        bool ok = true;

        for (int l = 0; l < mC->col && ok; ++l)
            for (int j = 0; j < mC->row && ok; ++j) {
                mC->data[l][j]=0;
                for (int k = 0; k < mA->col; ++k) {
                    mC->data[l][j] += mA->data[k][j] * mB->data[l][k];
                }
                ok = (mC->data[l][j] == mCTrue->data[l][j]);
            }
        if (ok)
            printf("okey");
        else
            printf("wrong");
        printf("\n");
        freeMatrixInt(mA);
        freeMatrixInt(mB);
        freeMatrixInt(mC);
        freeMatrixInt(mCTrue);
    }
}


int main(int argc, char **argv) {
    char listFileName[300];
    if (argc == 1)
        printf("No Extra Command Line Argument Passed\n");
    if (argc < 2)
        showError("Not Enough args");
    if (argc >= 2) {
        int argument = 0;
        if (sscanf(argv[++argument], "%s", listFileName) != 1)
            showError("Not String");

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
            mNames[i][j] = calloc(50, sizeof(char *));
        }
    }
    rewind(listFile);
    int counter = 0;
    while ((getline(&line, &len, listFile)) != -1) {
        sscanf(line, "%s %s %s", mNames[0][counter], mNames[1][counter], mNames[2][counter]);
        counter++;
    }

//    MatrixInt * m1 = fileToMatrixInt(mNames[0][0]);
//    if(!m1)
//        showError("Matrix elooo");

    multiplyProcess(mNames, lineCount);

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
    cEnd = clock();
    printf("%f\n", calcTime(cStart, cEnd));
    return 0;
}
