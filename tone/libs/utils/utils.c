#include "utils.h"

void error(char *message){
    perror(message);
    exit(-1);
}

void allocateVector(int** vector, int size) {
    *vector = (int*) calloc(size, sizeof(int));
    if(!*vector) { error("Error allocating memory (calloc) for the vector."); }
}

void freeVector(int* vector) {
    free(vector);
}

void allocateMatrix(int*** matrix, int rows, int cols) {
    *matrix = (int**) calloc(rows, sizeof(int*));
    if(!*matrix) { error("Error allocating memory (calloc) for the matrix."); }
    
    for (int i = 0; i < rows; i++) {
        (*matrix)[i] = (int*) calloc(cols, sizeof(int));
        if (!(*matrix)[i]) { error("Error allocating memory (calloc) for matrix row.\n") }
    }
}

void freeMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void readVectorFromFile(char *filename, int **vector) {
    int c, numero, totalNumeros;
    FILE *infile;
    infile = fopen(filename, "r");

    if(!infile) { error("Error opening file.\n"); }

    fscanf(infile, "%d", &totalNumeros);

    *vector = (int *)calloc(totalNumeros, sizeof(int));
    if(!*vector) { error("Error allocating memory (calloc) for the vector."); }

    for(c = 0; c < totalNumeros; c++) {
        fscanf(infile, "%d", &numero);
        (*vector)[c] = numero;
    }

    fclose(infile);
}

void writeVectorToFile(char *filename, int *vector, int size) {
    FILE *outfile = fopen(filename, "w");
    if (!outfile) { error("Error opening file.\n"); }

    fprintf(outfile, "%d\n", size);

    for (int i = 0; i < size; i++) {
        fprintf(outfile, "%d\n", vector[i]);
    }

    fclose(outfile);
}

void printVector(int *vector, int size) {
    printf("Vector elements: \n");
    for (int i = 0; i < size; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");
}

int sumVectorElementsInRange(int *vector, int startIndex, int endIndex) {
    int sum = 0;
    for (int i = startIndex; i < endIndex; i++) {
        sum += vector[i];
    }
    return sum;
}