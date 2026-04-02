#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// TODO

#define THREADS_NUM 5

sem_t sem;

typedef struct {
    pthread_t ID;
    int name;
} Thread_data;

void* job(void* arg) {
    Thread_data data = *(Thread_data*) arg;
    return NULL;
}

int main () {
    Thread_data threads[THREADS_NUM];
    sem_init(&sem, 0, 3);

    for (int i = 0; i < THREADS_NUM; i++) {
        pthread_create(&threads[i].ID, NULL, job, &threads[i]);
    }

    for (int i = 0; i < THREADS_NUM; i++) {
        pthread_join(threads[i].ID, NULL);
    }

    sem_destroy(&sem);
    return 0;
}
