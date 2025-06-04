#include <stdio.h>     
#include <stdlib.h>    
#include <string.h>    
#include <unistd.h>    
#include <signal.h>    
#include <sys/types.h> 
#include <sys/wait.h>  
#include <sys/shm.h>   
#include <sys/stat.h>  
#include <sys/ipc.h>
#include <pthread.h>
#include <stdbool.h>

char *fileName = "transactions.in",
**transactions = NULL,
*buffer;
int count = 0;

pthread_barrier_t mybarrier;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
int turno = 0;

void error(char *message){
    perror(message);
    exit(-1);
}

void loadTransactions (const char* fileName, char ***transactions, int *count) {
    FILE *infile = fopen(fileName, "r");
    if (!infile) {
        error("Error opening file.\n");
    }

    int temporalCount, max = 1024;
    if (fscanf(infile, "%d", &temporalCount) != 1) {
        fclose(infile);
        error("Error reading transactions dimensions.\n");
    }

    *transactions = (char**) calloc(temporalCount, sizeof(char*));
    if (!*transactions) {
        fclose(infile);
        error("Error allocating memory (calloc) for the transactions.");
    }

    for (int i = 0; i < temporalCount; i++) {
        (*transactions)[i] = (char*) calloc(max, sizeof(char));
        if (!(*transactions)[i]) {
            fclose(infile);
            error("Error allocating memory (calloc) for transactions row.\n");
        }
    }

    char buffer[1042];

    for (int i = 0; i < temporalCount; i++) {
        if(i == 0) fgets(buffer, max, infile);
        fgets((*transactions)[i], max, infile);
    }

    fclose(infile);
    *count = temporalCount;
}

void* basicFilter(void* arg) {
    while (1) {
        pthread_barrier_wait(&mybarrier);
        if (buffer == NULL) break;

        pthread_mutex_lock(&mutex);
        while (turno != 0)
            pthread_cond_wait(&cond, &mutex);

        if (!(strstr(buffer, "TRX:") && strstr(buffer, "->") && strstr(buffer, ":"))) {
            strcat(buffer, " RECHAZADO");
        }

        turno = 1;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

void* consistantVerify(void* arg) {
    while (1) {
        pthread_barrier_wait(&mybarrier);
        if (buffer == NULL) break;

        pthread_mutex_lock(&mutex);
        while (turno != 1)
            pthread_cond_wait(&cond, &mutex);

        char *delimitador = strrchr(buffer, ':');
        if (delimitador) {
            double amount = atof(delimitador + 1);
            if (amount < 0 || amount > 10000) {
                strcat(buffer, " RECHAZADO");
            }
        } else {
            strcat(buffer, " RECHAZADO");
        }

        turno = 2;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

void* rareDetection(void* arg) {
    while (1) {
        pthread_barrier_wait(&mybarrier);
        if (buffer == NULL) break;

        pthread_mutex_lock(&mutex);
        while (turno != 2)
            pthread_cond_wait(&cond, &mutex);

        if (strstr(buffer, "hacker") || strstr(buffer, "cashout") || strstr(buffer, "vault")) {
            strcat(buffer, " RECHAZADO");
        }

        turno = 0;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

int main(int argc, char* argv[]) {
    int nthreads = 3;

    pthread_t threads[nthreads];

    pthread_barrier_init(&mybarrier, NULL, 4);

    pthread_create(&threads[0], NULL, basicFilter, NULL);
    pthread_create(&threads[1], NULL, consistantVerify, NULL);
    pthread_create(&threads[2], NULL, rareDetection, NULL);

    loadTransactions(fileName, &transactions, &count);

    for(int index = 0; index < count; index++) {
        buffer = transactions[index];

        pthread_barrier_wait(&mybarrier);

        if (!strstr(buffer, "RECHAZADO")) {
            printf("%s", buffer);
        }
    }

    buffer = NULL;
    pthread_barrier_wait(&mybarrier);

    for(int i = 0; i < nthreads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    pthread_barrier_destroy(&mybarrier);
    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;
}