#include <pthread.h>
#include <stdio.h>

int NUM_THREADS = 5;

typedef struct {
    pthread_t id;
    int name;
} Thread_data;

void* job(void* arg) {
    Thread_data* data = (Thread_data*) arg;
    printf("Initializing T%d\t[%lld]...\n", data->name, pthread_self());
    return NULL;
}

int main() {
    Thread_data threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].name = i;
        pthread_create(&threads[i].id, NULL, job, &threads[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i].id, NULL);
    }

    return 0;
}
