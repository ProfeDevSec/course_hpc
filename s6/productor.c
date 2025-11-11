#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    sem_t empty; // Semáforo que controla si hay espacio para producir
    pthread_mutex_t mutex; // Mutex para evitar accesos concurrentes
} MessageQueue;

void initQueue(MessageQueue *queue) {
    queue->in = 0; // Posición inicial para insertar
    sem_init(&queue->empty, 0, BUFFER_SIZE); // Inicialmente el buffer está vacío
    pthread_mutex_init(&queue->mutex, NULL); // Inicializamos el mutex
}

void* producer(void *arg) {
    MessageQueue *queue = (MessageQueue*)arg;
    for (int i = 0; i < 10; i++) {
        // Esperar hasta que haya espacio disponible en el buffer
        sem_wait(&queue->empty);
        // Bloquea el acceso al buffer
        pthread_mutex_lock(&queue->mutex);
        // Generar y añadir un mensaje a la cola
        queue->buffer[queue->in] = i;
        printf("Productor ha creado el mensaje %d\n", i);
        queue->in = (queue->in + 1) % BUFFER_SIZE;
        // Desbloquear el acceso al buffer
        pthread_mutex_unlock(&queue->mutex);
        sleep(1); // Simula el tiempo para producir el próximo mensaje
    }
    return NULL;
}

int main() {
    MessageQueue queue;
    initQueue(&queue);
    pthread_t producerThread;
    // Crear e iniciar el hilo del productor
    pthread_create(&producerThread, NULL, producer, (void*)&queue);
    // Esperar a que el hilo del productor finalice
    pthread_join(producerThread, NULL);
    printf("El productor ha terminado de crear mensajes.\n");
    // Destruir semáforos y mutex
    sem_destroy(&queue.empty);
    pthread_mutex_destroy(&queue.mutex);
    return 0;
}
