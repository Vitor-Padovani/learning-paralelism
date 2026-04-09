#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <time.h>

#define NUM_THREADS 1
#define SIZE 100000000

struct timespec start, end;

long* vector;

typedef struct {
    pthread_t id;
    int num;
    int section_start;
    int section_end;
    long local_min;
    long local_max;
} Thread_data;

void print_vector(long* vector, int size) {
    for (int i = 0; i < size; i++)
        printf("%ld ", vector[i]);
    printf("\n");
}

void* job(void* arg) {
    Thread_data* thread = (Thread_data*) arg;

    printf("Thread %d iniciada [%lu]\n", thread->num, pthread_self());

    int start = thread->section_start;
    int end = thread->section_end;

    // Inicializa com o primeiro elemento da seção
    thread->local_min = vector[start];
    thread->local_max = vector[start];

    for (int i = start + 1; i < end; i++) {
        if (vector[i] < thread->local_min)
            thread->local_min = vector[i];

        if (vector[i] > thread->local_max)
            thread->local_max = vector[i];
    }

    printf("Thread %d terminou: min=%ld, max=%ld\n",
           thread->num, thread->local_min, thread->local_max);

    return NULL;
}

int main() {
    srand(time(NULL));

    Thread_data threads[NUM_THREADS];

    vector = (long*) malloc(SIZE * sizeof(long));

    // Preenche vetor
    for (int i = 0; i < SIZE; i++)
        vector[i] = rand() % 100000000;

    //printf("Vetor:\n");
    //print_vector(vector, SIZE);

    clock_gettime(CLOCK_MONOTONIC, &start);

    // Criação das threads
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].num = i;
        threads[i].section_start = (SIZE / NUM_THREADS) * i;
        threads[i].section_end = (i == NUM_THREADS - 1)
            ? SIZE
            : (SIZE / NUM_THREADS) * (i + 1);

        pthread_create(&threads[i].id, NULL, job, &threads[i]);
    }

    // Espera threads terminarem
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i].id, NULL);
    }

    // Redução final (combinar resultados)
    long global_min = threads[0].local_min;
    long global_max = threads[0].local_max;

    for (int i = 1; i < NUM_THREADS; i++) {
        if (threads[i].local_min < global_min)
            global_min = threads[i].local_min;

        if (threads[i].local_max > global_max)
            global_max = threads[i].local_max;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = (end.tv_sec - start.tv_sec) +
                 (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Tempo de execução: %f segundos\n", elapsed);

    printf("\nResultado final:\n");
    printf("Menor elemento: %ld\n", global_min);
    printf("Maior elemento: %ld\n", global_max);

    free(vector);

    return 0;
}