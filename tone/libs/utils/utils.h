#ifndef TONE_LIBS_UTILS_H
#define TONE_LIBS_UTILS_H

#include <stdio.h>      // Para funciones de entrada/salida como printf.
#include <stdlib.h>     // Para funciones de utilidad general como exit y system.
#include <string.h>     // Para manejar los Strings.
#include <sys/types.h>  // Para tipos de datos como pid_t.
#include <sys/wait.h>   // Para la función wait.
#include <unistd.h>     // Para la función fork y sleep.

// Función para manejar errores
void error(const char *msg);

// Función para leer un vector desde un archivo
int readVectorFromFile(char *filename, int **vec);

// Función para escribir un vector en un archivo
void writeVectorToFile(const char *fileName, int **vec, size_t size);

// Función para imprimir un vector
void printVector(int **vec, size_t size);

//Función para sumar los valores de un vector en un rango
int sumVectorRange(int **vec, int startIndex, int endIndex)

#endif // TONE_LIBS_UTILS_H