#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> [20]

typedef struct { [21]
float* a;
    float* b;
    float* result;
    int start;
    int end;
} ThreadData;

void* parallel_addition(void* arg) { [22]
ThreadData* data = (ThreadData*) arg;
    for (int i = data->start; i < data->end; ++i) {
        data->result[i] = data->a[i] + data->b[i];
    }
    return NULL;
}

int main() { [22, 23]
const int size = 8;
    const int num_threads = 4;
    float a[size] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    float b[size] = {2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    float result[size] = {0};
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    int chunk_size = size / num_threads;
    // Iniciar los hilos [24]
    for (int i = 0; i < num_threads; ++i) {
        thread_data[i].a = a;
        thread_data[i].b = b;
        thread_data[i].result = result;
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == num_threads - 1) ? size : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, parallel_addition, &thread_data[i]);
    }
    // Esperar a que todos los hilos terminen
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    // Imprimir los resultados [25]
    for (int i = 0; i < size; ++i) {
        printf("Resultado[%d]: %.1f\n", i, result[i]);
    }
    return 0;
}