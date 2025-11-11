#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
typedef struct {
    int buffer[BUFFER_SIZE];
    int out;
    sem_t full; // Semáforo que indica si hay mensajes disponibles para consumir
    pthread_mutex_t mutex; // Mutex para evitar accesos concurrentes
} MessageQueue;

void initQueue(MessageQueue *queue) {
    queue->out = 0; // Posición inicial para consumir
    sem_init(&queue->full, 0, 0); // Inicialmente no hay mensajes disponibles
    pthread_mutex_init(&queue->mutex, NULL); // Inicializamos el mutex
}


void* consumer(void *arg) {
    MessageQueue *queue = (MessageQueue*)arg;
    for (int i = 0; i < 10; i++) {
        // Espera hasta que haya mensajes disponibles en el buffer
        sem_wait(&queue->full);
        // Bloquea el acceso al buffer
        pthread_mutex_lock(&queue->mutex);
        // Consume el mensaje desde la cola
        int message = queue->buffer[queue->out];
        printf("Consumidor ha procesado el mensaje %d\n", message);
        queue->out = (queue->out + 1) % BUFFER_SIZE;
        // Desbloquea el acceso al buffer
        pthread_mutex_unlock(&queue->mutex);
        sleep(1); // Simula tiempo para procesar el mensaje
    }
    return NULL;
}

int main() {
    MessageQueue queue;
    initQueue(&queue);
    pthread_t consumerThread;
    // Crear e iniciar el hilo del consumidor
    pthread_create(&consumerThread, NULL, consumer, (void*)&queue);
    // Esperar a que el hilo del consumidor finalice
    pthread_join(consumerThread, NULL);
    printf("El consumidor ha terminado de procesar mensajes.\n");
    // Destruir semáforos y mutex
    sem_destroy(&queue.full);
    pthread_mutex_destroy(&queue.mutex);
    return 0;
}

