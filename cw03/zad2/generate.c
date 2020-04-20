#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

extern int errno;

int MIN, MAX, COUNT;


void showError(char *msg) {
    printf("---ERROR:  ");
    printf("%s", msg);
    printf("\n");
    exit(-1);
}

int randInt(int lower, int upper) {
    return (rand() % (upper - lower + 1)) + lower;
}

int getRandDim() {
    return randInt(MIN, MAX);
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

void stringToFileMatrixInt(MatrixInt *m, char *name) {

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


void fillRandMatrixInt(MatrixInt *m) {
    for (int i = 0; i < m->col; ++i) {
        for (int j = 0; j < m->row; ++j) {
            m->data[i][j] = randInt(0, 10);
        }
    }
}

void createMatrixPair(int id, FILE * list ) {

    char mNA[50];
    char mNB[50];
    sprintf(mNA, "matrixA%i", id);
    sprintf(mNB, "matrixB%i", id);
    int rowA = getRandDim();
    int colA = getRandDim();
    int rowB = colA;
    int colB = getRandDim();

    MatrixInt *mA = createMatrixInt(colA, rowA);
    fillRandMatrixInt(mA);
    stringToFileMatrixInt(mA, mNA);
    freeMatrixInt(mA);

    MatrixInt *mB = createMatrixInt(colB, rowB);
    fillRandMatrixInt(mB);
    stringToFileMatrixInt(mB, mNB);
    freeMatrixInt(mB);

    int length = 200;
    char *line = calloc(length, sizeof(char));
    sprintf(line, "%s %s matrixC%i\n", mNA,mNB,id);
    fwrite(line, sizeof(char), strlen(line), list);
    free(line);

}

int main(int argc, char **argv) {
    if (argc == 1)
        printf("No Extra Command Line Argument Passed\n");
    if (argc < 3)
        showError("Not Enough args");
    if (argc >= 3) {
        int argument = 0;
        if (sscanf(argv[++argument], "%i", &COUNT) != 1)
            showError("Not int");
        if (COUNT < 0)
            showError("Negative number");
        if (sscanf(argv[++argument], "%i", &MIN) != 1)
            showError("Not int");
        if (MIN < 0)
            showError("Negative number");
        if (sscanf(argv[++argument], "%i", &MAX) != 1)
            showError("Not int");
        if (MAX < 0)
            showError("Negative number");
        if (MAX < MIN) {
            int swapInt = MAX;
            MAX = MIN;
            MIN = swapInt;
        }

        FILE *listFile = fopen("list", "w+");
        if (listFile == NULL)
            perror("fopen listFile");

        for (int i = 0; i < COUNT; ++i) {
            createMatrixPair(i,listFile);
        }

        fclose(listFile);

    }
    return 0;
}
