#include <pthread.h>
#include <stdio.h>

int nth_fib = 5; // nth fib term
int a = 1;
int b = 1;
int button = 0;

typedef struct {
    pthread_t id;
    int name;
} Thread_data;

pthread_mutex_t lock;

void* job(void* arg) {
    Thread_data* data = (Thread_data*) arg;

    printf("Initializing T%d\t[%lu]...\n", data->name, (unsigned long)pthread_self());

    while (1) {
        pthread_mutex_lock(&lock);

        if (nth_fib <= 0) {
            pthread_mutex_unlock(&lock);
            break;
        }

        if (button == 0) {
            a += b;
            button = 1;
        } else {
            b += a;
            button = 0;
        }

        nth_fib--;

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    Thread_data threads[2];
    
    pthread_mutex_init(&lock, NULL);
    nth_fib--;

    for (int i = 0; i < 2; i++) {
        threads[i].name = i;
        pthread_create(&threads[i].id, NULL, job, &threads[i]);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i].id, NULL);
    }

    pthread_mutex_destroy(&lock);

    printf("Fibonacci: %d\n", (a > b)? a : b);

    return 0;
}
