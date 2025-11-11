#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define QUEUE_SIZE 10
typedef struct {
    int buffer[QUEUE_SIZE];
    int in;
    int out;
    sem_t full; // Semáforo que indica si la cola está llena
    sem_t empty; // Semáforo que indica si la cola está vacía
    pthread_mutex_t mutex; // Mutex para proteger el acceso a la cola
} MessageQueue;

void initQueue(MessageQueue *queue) {
    queue->in = 0;
    queue->out = 0;
    sem_init(&queue->full, 0, 0); // Inicialmente la cola está vacía
    sem_init(&queue->empty, 0, QUEUE_SIZE); // Hay espacio para QUEUE_SIZE mensajes
    pthread_mutex_init(&queue->mutex, NULL); // Inicializamos el mutex
}

void* producer(void *arg) {
    MessageQueue *queue = (MessageQueue*)arg;
    for (int i = 0; i < 10; i++) {
        sem_wait(&queue->empty); // Espera si la cola está llena
        pthread_mutex_lock(&queue->mutex); // Bloquea el acceso a la cola
        // Produce un mensaje
        queue->buffer[queue->in] = i;
        printf("Productor ha creado el mensaje %d\n", i);
        queue->in = (queue->in + 1) % QUEUE_SIZE;
        pthread_mutex_unlock(&queue->mutex); // Desbloquea la cola
        sem_post(&queue->full); // Señala que hay un nuevo mensaje disponible
        sleep(1); // Simula tiempo para producir el próximo mensaje
    }
    return NULL;
}

void* consumer(void *arg) {
    MessageQueue *queue = (MessageQueue*)arg;
    for (int i = 0; i < 10; i++) {
        sem_wait(&queue->full); // Espera si la cola está vacía
        pthread_mutex_lock(&queue->mutex); // Bloquea el acceso a la cola
        // Consume un mensaje
        int message = queue->buffer[queue->out];
        printf("Consumidor ha procesado el mensaje %d\n", message);
        queue->out = (queue->out + 1) % QUEUE_SIZE;
        pthread_mutex_unlock(&queue->mutex); // Desbloquea la cola
        sem_post(&queue->empty); // Señala que hay espacio disponible en la cola
        sleep(1); // Simula tiempo para procesar el próximo mensaje
    }
    return NULL;
}


int main() {
    MessageQueue queue;
    initQueue(&queue);
    pthread_t producerThread, consumerThread;
    // Crear los hilos del productor y consumidor
    pthread_create(&producerThread, NULL, producer, (void*)&queue);
    pthread_create(&consumerThread, NULL, consumer, (void*)&queue);
    // Esperar a que ambos hilos finalicen
    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);
    printf("Todos los mensajes han sido procesados.\n");
    // Destruir semáforos y mutex
    sem_destroy(&queue.full);
    sem_destroy(&queue.empty);
    pthread_mutex_destroy(&queue.mutex);
    return 0;
}