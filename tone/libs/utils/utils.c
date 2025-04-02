#include "utils.h"

void error(char *msg){
    perror(msg);
    exit(-1);
}

int readVectorFromFile(char *filename, int **vector) {
    int c, numero, totalNumeros;
    FILE *infile;
    infile = fopen(filename, "r");

    if(!infile) { error("Error fopen\n"); }

    fscanf(infile, "%d", &totalNumeros); 
    *vector = (int *)calloc(totalNumeros, sizeof(int));

    if(!*vector) { error("Error calloc"); }

    for(c = 0; c < totalNumeros; c++) {
        fscanf(infile, "%d", &numero);
        (*vector)[c] = numero;
    }

    fclose(infile);

    return c;  
}

void writeVectorToFile(const char *fileName, int **vector, size_t size) {
    FILE *infile = fopen(fileName, "w");
    if (!infile) { error("Error fopen\n"); }
    
    for (size_t i = 0; i < size; i++) {
        fprintf(infile, "%d\n", (*vector)[i]);
    }
    
    fclose(infile);
}

void printVector(int **vector, size_t size) {
    printf("Vector elements:\n");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", (*vector)[i]);
    }
    printf("\n");
}

int sumVectorRange(int **vector, int startIndex, int endIndex) {
    int sum = 0;
    for(int i = startIndex; i < endIndex; i++) {
        sum += (*vector)[i];
    }
    return sum;
}