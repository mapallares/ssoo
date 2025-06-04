#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>      // Para funciones de entrada/salida como printf.
#include <stdlib.h>     // Para funciones de utilidad general como exit y system.
#include <string.h>     // Para manejar los Strings.
#include <unistd.h>     // Para la función fork y sleep.
#include <signal.h>     // Para la comunicación con señales.
#include <sys/types.h>  // Para tipos de datos como pid_t.
#include <sys/wait.h>   // Para la función wait.
#include <sys/shm.h>    // Para la memoria compartida shmget, shmat, shmdt y shmctl.
#include <sys/stat.h>   // Para las constantes S_IRUSR y S_IWUSR.
#include <sys/ipc.h>    // Para la comunicación entre procesos IPC.
#include <pthread.h>    // Incluye las funciones y tipos para hilos POSIX


void error(char *message){
    perror(message);
    exit(-1);
}

void showProcessTree(){
    char b[50]; 
    sprintf(b,"pstree -lp %d", getpid());
    system(b);
}

int KILL(pid_t pid, int signal) {
    usleep(10000);
    return kill(pid, signal);
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
        if (!(*matrix)[i]) { error("Error allocating memory (calloc) for matrix row.\n"); }
    }
}

void freeMatrix(int** matrix, int rows) {
    if (!matrix) return;
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int readVectorFromFile(char *fileName, int **vector) {
    int c, numero, totalNumeros;
    FILE *infile;
    infile = fopen(fileName, "r");

    if(!infile) { error("Error opening file.\n"); }

    fscanf(infile, "%d", &totalNumeros);

    *vector = (int *)calloc(totalNumeros, sizeof(int));
    if(!*vector) { error("Error allocating memory (calloc) for the vector."); }

    for(c = 0; c < totalNumeros; c++) {
        fscanf(infile, "%d", &numero);
        (*vector)[c] = numero;
    }

    fclose(infile);

    return c;
}

void writeVectorToFile(char *fileName, int *vector, int size) {
    FILE *outfile = fopen(fileName, "w");
    if (!outfile) { error("Error opening file.\n"); }

    fprintf(outfile, "%d\n", size);

    for (int i = 0; i < size; i++) {
        fprintf(outfile, "%d\n", vector[i]);
    }

    fclose(outfile);
}

void printVector(int *vector, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");
}

int sumVectorElementsInRange(int *vector, int startIndex, int endIndex) {
    int sum = 0;
    for (int i = startIndex; i <= endIndex; i++) {
        sum += vector[i];
    }
    return sum;
}

void readMatrixFromFile(const char* fileName, int ***matrix, int *rows, int *cols) {
    FILE *infile = fopen(fileName, "r");
    if (!infile) {
        error("Error opening file.\n");
    }

    int temporalRows, temporalCols;
    if (fscanf(infile, "%d %d", &temporalRows, &temporalCols) != 2) {
        fclose(infile);
        error("Error reading matrix dimensions.\n");
    }

    *matrix = (int**) calloc(temporalRows, sizeof(int*));
    if (!*matrix) {
        fclose(infile);
        error("Error allocating memory (calloc) for the matrix.");
    }

    for (int i = 0; i < temporalRows; i++) {
        (*matrix)[i] = (int*) calloc(temporalCols, sizeof(int));
        if (!(*matrix)[i]) {
            fclose(infile);
            error("Error allocating memory (calloc) for matrix row.\n");
        }
    }

    for (int i = 0; i < temporalRows; i++) {
        for (int j = 0; j < temporalCols; j++) {
            if (fscanf(infile, "%d", &(*matrix)[i][j]) != 1) {
                fclose(infile);
                error("Error reading matrix data.\n");
            }
        }
    }

    fclose(infile);
    *rows = temporalRows;
    *cols = temporalCols;
}

void printMatrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void traverseDiagonal(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows && i < cols; i++) {
        printf("(%d,%d): %d\n", i, i, matrix[i][i]);
    }
}

void traverseUpperTriangular(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = i; j < cols; j++) {
            printf("(%d,%d): %d\n", i, j, matrix[i][j]);
        }
    }
}

void traverseLowerTriangular(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= i && j < cols; j++) {
            printf("(%d,%d): %d\n", i, j, matrix[i][j]);
        }
    }
}

void traverseSpiral(int** matrix, int rows, int cols) {
    int top = 0, bottom = rows - 1, left = 0, right = cols - 1;

    while (top <= bottom && left <= right) {
        for (int i = left; i <= right; i++) {
            printf("(%d,%d): %d\n", top, i, matrix[top][i]);
        }
        top++;

        for (int i = top; i <= bottom; i++) {
            printf("(%d,%d): %d\n", i, right, matrix[i][right]);
        }
        right--;

        if (top <= bottom) {
            for (int i = right; i >= left; i--) {
                printf("(%d,%d): %d\n", bottom, i, matrix[bottom][i]);
            }
            bottom--;
        }

        if (left <= right) {
            for (int i = bottom; i >= top; i--) {
                printf("(%d,%d): %d\n", i, left, matrix[i][left]);
            }
            left++;
        }
    }
}

int searchHorizontal(char **matrix, int rows, int cols, char *word) {
    int len = strlen(word);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= cols - len; j++) {
            int k;
            for (k = 0; k < len; k++)
                if (matrix[i][j + k] != word[k]) break;
            if (k == len) return 1;

            for (k = 0; k < len; k++)
                if (matrix[i][j + k] != word[len - 1 - k]) break;
            if (k == len) return 1;
        }
    }
    return 0;
}

int searchVertical(char **matrix, int rows, int cols, char *word) {
    int len = strlen(word);
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i <= rows - len; i++) {
            int k;
            for (k = 0; k < len; k++)
                if (matrix[i + k][j] != word[k]) break;
            if (k == len) return 1;

            for (k = 0; k < len; k++)
                if (matrix[i + k][j] != word[len - 1 - k]) break;
            if (k == len) return 1;
        }
    }
    return 0;
}

int searchDiagonal(char **matrix, int rows, int cols, char *word) {
    int len = strlen(word);

    for (int i = 0; i <= rows - len; i++) {
        for (int j = 0; j <= cols - len; j++) {
            int k;
            for (k = 0; k < len; k++)
                if (matrix[i + k][j + k] != word[k]) break;
            if (k == len) return 1;

            for (k = 0; k < len; k++)
                if (matrix[i + k][j + k] != word[len - 1 - k]) break;
            if (k == len) return 1;
        }
    }

    for (int i = 0; i <= rows - len; i++) {
        for (int j = len - 1; j < cols; j++) {
            int k;
            for (k = 0; k < len; k++)
                if (matrix[i + k][j - k] != word[k]) break;
            if (k == len) return 1;

            for (k = 0; k < len; k++)
                if (matrix[i + k][j - k] != word[len - 1 - k]) break;
            if (k == len) return 1;
        }
    }

    return 0;
}

void reverseString(char *str) {
    int start = 0;
    int end = strlen(str) - 1;

    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void readFile(const char* fileName, char** contentOut, int* fileSize) {
    FILE* file = fopen(fileName, "r");
    if (!file) {
        perror("File not found\n");
        *contentOut = NULL;
        return;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char* content = (char*)malloc(size + 1);
    if (!content) {
        perror("Memory error\n");
        fclose(file);
        *contentOut = NULL;
        return;
    }

    fread(content, 1, size, file);
    content[size] = '\0';

    fclose(file);
    *fileSize = (int)size;
    *contentOut = content;
}

void readFileLineByLine(const char* fileName, char** contentOut, int* fileSize) {
    FILE* file = fopen(fileName, "r");
    if (!file) {
        perror("File not found\n");
        *contentOut = NULL;
        return;
    }

    int maxSize = 1024;
    char buffer[1024];
    int length = 0;

    char* content = (char*)malloc(maxSize);
    if (!content) {
        perror("Memory error\n");
        fclose(file);
        *contentOut = NULL;
        return;
    }

    content[0] = '\0';

    while (fgets(buffer, sizeof(buffer), file)) {
        size_t lineLen = strlen(buffer);

        if (length + lineLen + 1 > maxSize) {
            maxSize *= 2;
            char* temp = (char*)realloc(content, maxSize);
            if (!temp) {
                perror("Failed to reallocate memory");
                free(content);
                fclose(file);
                *contentOut = NULL;
                return;
            }
            content = temp;
        }

        strcpy(content + length, buffer);
        length += lineLen;
    }

    fclose(file);
    *fileSize = length;
    *contentOut = content;
}

void findPatternOccurrences(const char* text, const char* pattern, int* positions, int* count) {
    int textLen = strlen(text);
    int patternLen = strlen(pattern);
    int pos = 0;
    *count = 0;

    if (patternLen == 0) return;

    while (pos <= textLen - patternLen) {
        if (strncmp(&text[pos], pattern, patternLen) == 0) {
            positions[*count] = pos;
            (*count)++;
            pos += patternLen;
        } else {
            pos++;
        }
    }
}

void findOccurrences(const char *text, const char *pattern, int **results, int *count) {
    size_t textLength = strlen(text);
    size_t patternLength = strlen(pattern);

    if (patternLength == 0 || patternLength > textLength) {
        *results = NULL;
        *count = 0;
        return;
    }

    int capacity = 10;
    int *positions = (int*) malloc(capacity * sizeof(int));
    if (!positions) {
        perror("Memory allocation error");
        *results = NULL;
        *count = 0;
        return;
    }

    int total = 0;
    for (int i = 0; i <= (int)(textLength - patternLength); i++) {
        if (strncmp(&text[i], pattern, patternLength) == 0) {
            if (total >= capacity) {
                capacity *= 2;
                int *temp = (int*) realloc(positions, capacity * sizeof(int));
                if (!temp) {
                    perror("Memory reallocation error");
                    free(positions);
                    *results = NULL;
                    *count = 0;
                    return;
                }
                positions = temp;
            }
            positions[total++] = i;
        }
    }

    *results = positions;
    *count = total;
}

void sumWindows(int** matrix, int rows, int cols, int windowHeight, int windowWidth) {
    int halfHeight = windowHeight / 2;
    int halfWidth = windowWidth / 2;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {

            int sum = 0;

            for (int di = -halfHeight; di <= halfHeight; di++) {
                for (int dj = -halfWidth; dj <= halfWidth; dj++) {
                    int ni = i + di;
                    int nj = j + dj;

                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                        sum += matrix[ni][nj];  // Perform the required operation (sum)
                    }
                }
            }

            printf("Sum at (%d,%d): %d\n", i, j, sum);
        }
    }
}

int readCharVectorFromFile(char *fileName, char **vector) {
    int c, len;
    char charter;
    FILE *infile;
    infile = fopen(fileName, "r");

    if(!infile) { error("Error opening file.\n"); }

    fscanf(infile, "%d", &len);

    *vector = (char *)calloc(len, sizeof(char));
    if(!*vector) { error("Error allocating memory (calloc) for the vector."); }

    for(c = 0; c < len; c++) {
        fscanf(infile, "%c", &charter);
        (*vector)[c] = charter;
    }

    fclose(infile);

    return c;
}

void writeCharVectorToFile(char *fileName, char *vector, int size) {
    FILE *outfile = fopen(fileName, "w");
    if (!outfile) { error("Error opening file.\n"); }

    fprintf(outfile, "%d\n", size);

    for (int i = 0; i < size; i++) {
        fprintf(outfile, "%c\n", vector[i]);
    }

    fclose(outfile);
}

void printCharVector(char *vector, int size) {
    for (int i = 0; i < size; i++) {
        printf("%c ", vector[i]);
    }
    printf("\n");
}

unsigned int sizeOfMatrix(int rows, int cols, size_t sizeElement) {
    size_t size;
    size = rows * sizeof(void *);
    size += (cols * rows * sizeElement);
    return size;
}

void createIndex(void **matrix, int rows, int cols,  size_t sizeElement) {
    int index;
    size_t sizeRow = cols * sizeElement;
    matrix[0] = matrix + rows;
    for(index = 1; index < rows; index++) {
        matrix[index] = (matrix[index - 1] + sizeRow);
    }
}

void combSort(int **vec, int n) {
    int gap = n;
    int swapped = 1;

    while (gap != 1 || swapped == 1) {
        gap = gap / 1.3;
        if (gap < 1) {
            gap = 1;
        }

        swapped = 0;

        for (int i = 0; i < n - gap; i++) {
            if ((*vec)[i] > (*vec)[i + gap]) {
                int temp = (*vec)[i];
                (*vec)[i] = (*vec)[i + gap];
                (*vec)[i + gap] = temp;
                swapped = 1;
            }
        }
    }
}

#endif