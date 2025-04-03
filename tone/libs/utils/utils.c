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

void writeVectorToFile(char *fileName, int *vector, int size) {
    FILE *outfile = fopen(fileName, "w");
    if (!outfile) {
        error("Error fopen\n");
    }

    fprintf(outfile, "%d\n", size);

    for (int i = 0; i < size; i++) {
        fprintf(outfile, "%d\n", vector[i]);
    }

    fclose(outfile);
}

void printVector(int *vector, int size) {
    printf("Vector elements:\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");
}

int sumVectorRange(int *vector, int startIndex, int endIndex) {
    int sum = 0;
    for (int i = startIndex; i < endIndex; i++) {
        sum += vector[i];
    }
    return sum;
}