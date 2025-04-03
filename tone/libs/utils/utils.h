#ifndef TONE_LIBS_UTILS_H
#define TONE_LIBS_UTILS_H

#include <stdio.h>      // Para funciones de entrada/salida como printf.
#include <stdlib.h>     // Para funciones de utilidad general como exit y system.
#include <string.h>     // Para manejar los Strings.
#include <sys/types.h>  // Para tipos de datos como pid_t.
#include <sys/wait.h>   // Para la función wait.
#include <unistd.h>     // Para la función fork y sleep.

// Función para manejar errores
void error(char *msg);

// Función para reservar memoria para un vector dinámico
void allocateVector(int** vector, int size);

// Función para liberar la memoria del vector
void freeVector(int* vector);

// Función para reservar memoria para una matriz dinámica
void allocateMatrix(int*** matrix, int rows, int cols);

// Función para liberar la memoria de la matriz
void freeMatrix(int** matrix, int rows);

// Función para leer un vector desde un archivo
void readVectorFromFile(char *filename, int **vector);

// Función para escribir un vector en un archivo
void writeVectorToFile(char *fileName, int *vector, int size);

// Función para imprimir un vector
void printVector(int *vector, int size);

//Función para sumar los valores de un vector en un rango
int sumVectorElementsInRange(int *vector, int startIndex, int endIndex);

#endif