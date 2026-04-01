#include <pthread.h>
#include <stdio.h>

int NUM_THREADS = 10;
int N = 100000;
int counter = 0;

pthread_mutex_t lock;

typedef struct {
    pthread_t id;
    int name;
} Thread_data;

void* job(void* arg) {
    Thread_data* data = (Thread_data*) arg;
    printf("Initializing T%d\t[%lld]...\n", data->name, pthread_self());

    for (int i = 0; i < N; i++) {
        //pthread_mutex_lock(&lock); // Without lock and unlock, the result is wrong
        counter++;
        //pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    Thread_data threads[NUM_THREADS];
    
    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].name = i;
        pthread_create(&threads[i].id, NULL, job, &threads[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i].id, NULL);
    }

    pthread_mutex_destroy(&lock);

    printf("Final sum: %d\n", counter);

    return 0;
}