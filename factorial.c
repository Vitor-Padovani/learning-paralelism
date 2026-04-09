#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3
#define FACT 10

long* fact_vector;

typedef struct {
    pthread_t id;
    int num;
    int section_start;
    int section_end;
} Thread_data;

void print_vector(long* vector, int size) {
    for (int i = 0; i < size; i++)
        printf("%d,", vector[i]);
    printf("\n");
}

void* job(void* arg) {
    Thread_data* thread = (Thread_data*) arg;
    // printf("Initializing T%d\t[%lld]...\n", thread->num, pthread_self());
    // printf("-> %d, %d\n", thread->section_start, thread->section_end);

    for (int i = thread->section_start+1; i < thread->section_end; i++) {
        fact_vector[thread->section_start] *= fact_vector[i];
    }

    if (thread->num == 0) {
        int remaining = FACT % NUM_THREADS;

        for (int i = 0; i < 99999999; i++);

        for (int i = FACT - remaining; i < FACT; i++)
            fact_vector[0] *= fact_vector[i];

        for (int i = 1; i < NUM_THREADS; i++)
            fact_vector[0] *= fact_vector[i*thread->section_end];
    }

    return NULL;
}

int main() {
    Thread_data threads[NUM_THREADS];

    fact_vector = (long*) malloc(FACT * sizeof(long));

    for (int i = 0; i < FACT; i++)
        fact_vector[i] = i+1;

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].num = i;
        threads[i].section_start = (FACT / NUM_THREADS) * threads[i].num;
        threads[i].section_end = (FACT / NUM_THREADS) * (threads[i].num + 1);
        pthread_create(&threads[i].id, NULL, job, &threads[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i].id, NULL);
    }

    printf("Factorial: %ld\n", fact_vector[0]);

    return 0;
}