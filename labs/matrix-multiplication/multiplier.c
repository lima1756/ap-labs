#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "logger.h"
#define NUM_THREADS 2000

long * readMatrix(char *filename);
long * getColumn(int col, long *matrix);
long * getRow(int row, long *matrix);
int getLock();
int releaseLock(int lock);
long dotProduct(long *vec1, long *vec2);
long * multiply(long *matA, long *matB);
int saveResultMatrix(long *result);

long **buffers;
int NUM_BUFFERS = 0;
pthread_mutex_t *mutexes;
pthread_t threads[NUM_THREADS];

struct args {
    int row;
    int col;
    long* matA;
    long* matB;
};

int main(int argc, char **argv){
    long *data1, *data2;
    data1 = readMatrix("./matA.dat");
    data2 = readMatrix("./matB.dat");
    if(argc == 3 && strcmp(argv[1], "-n") == 0){
        NUM_BUFFERS = atoi(argv[2]);
    } 
    if(NUM_BUFFERS == 0) {
        NUM_BUFFERS = 8;
    }
    buffers = malloc(NUM_BUFFERS * sizeof(long *));
    mutexes = malloc(NUM_BUFFERS * sizeof(pthread_mutex_t));
    for(int i = 0; i < NUM_BUFFERS; i++){
        pthread_mutex_init(&mutexes[i], NULL);
    }
    long *res = multiply(data1, data2);
    saveResultMatrix(res);
    free(buffers);
    free(mutexes);
    free(data1);
    free(data2);
    free(res);
    return 0;
}

long * readMatrix(char *filename) {
    long lines = 0;
    long *data;
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        return NULL;
    }
    // counting number of lines therefore number of data
    char c;
    while((c = fgetc(file)) != EOF){
        if(c=='\n')
            lines++;
    }

    rewind(file);
    data = malloc(lines* sizeof(long));
    if(data == NULL){
        return NULL;
    }
    int i = 0;
    long test;
    while(fscanf(file, "%ld", &data[i]) != EOF){
        i++;
    }
    fclose(file);
    return data;
}

long * getColumn(int col, long *matrix) {
    long *column = malloc(2000 * sizeof(long));
    for(int i = 0; i < 2000; i++){
        column[i] = matrix[i*2000 + col];
    }
    return column;
}

long * getRow(int r, long *matrix) {
    long *row = malloc(2000 * sizeof(long));
    for(int i = 0; i < 2000; i++){
        row[i] = matrix[r*2000 + i];
    }
    return row;
}

int getLock() {
    for(int i = 0; i < NUM_BUFFERS; i++){
        if(pthread_mutex_trylock(&mutexes[i])==0){
            return i;
        }   
    }
    return -1;
}

int releaseLock(int lock) {
    return pthread_mutex_unlock(&mutexes[lock]);
}

long dotProduct(long *vec1, long *vec2) {
    long sum = 0;
    for(int i = 0; i < 2000; i ++) {
        sum += (vec1[i] * vec2[i]);
    }
    return sum;
}

long doMultiplication(struct args *data){
    int bufA = -1, bufB = -1;
    while(bufA == -1 || bufB == -1){
        if(bufA == -1){
            bufA = getLock();
        }
        if(bufB == -1){
            bufB = getLock();
        }
    }
    buffers[bufA] = getRow(data->row, data->matA);
    buffers[bufB] = getColumn(data->col, data->matB);
    long res = dotProduct(buffers[bufA], buffers[bufB]);
    free(buffers[bufA]);
    free(buffers[bufB]);
    free(data);
    releaseLock(bufA);
    releaseLock(bufB);
    return res;
}

long * multiply(long *matA, long *matB) {
    long *data = malloc(4000000 * sizeof(long));
    for(int i = 0; i < NUM_THREADS; i++){
        printf("%d\n", i);
        for(int j = 0; j < NUM_THREADS; j++)
        {
            struct args *curr_args = malloc(sizeof(struct args));
            curr_args->row = i;
            curr_args->col = j;
            curr_args->matA = matA;
            curr_args->matB = matB;
            pthread_create(&threads[j], NULL, (void * (*)(void *))doMultiplication, (void *)curr_args);
        }

        for(int j = 0; j < NUM_THREADS; j++) {
            void *status;
            pthread_join(threads[j], &status);
            data[2000*j+i] = (long*) status;
        }
    }
    return data;
}

int saveResultMatrix(long *result) {    
    FILE *file;
    file = fopen("result.data", "w+");
    for(int i = 0; i < 4000000; i++) {
        fprintf(file, "%ld\n", result[i]);
    }
    fclose(file);
    return -1;
}