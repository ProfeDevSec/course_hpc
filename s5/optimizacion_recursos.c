#include <stdio.h> // Para funciones de entrada/salida
#include <pthread.h> // Para el manejo de hilos
#include <unistd.h> // Para la simulación de retrasos (usleep)

void* tarea_procesamiento_pesado(void* arg) {
    printf("Tarea de procesamiento pesado comenzando...\n");
    usleep(2000000); // Simula una tarea pesada que dura 2 segundos
    printf("Tarea de procesamiento pesado completada.\n");
    return NULL;
}
void* tarea_procesamiento_ligero(void* arg) {
    printf("Tarea de procesamiento ligero comenzando...\n");
    usleep(1000000); // Simula una tarea ligera que dura 1 segundo
    printf("Tarea de procesamiento ligero completada.\n");
    return NULL;
}

int main() {
    pthread_t hilo1, hilo2; // Identificadores de los hilos
    // Crear hilos para las dos tareas
    pthread_create(&hilo1, NULL, tarea_procesamiento_pesado, NULL);
    pthread_create(&hilo2, NULL, tarea_procesamiento_ligero, NULL);
    // Esperar a que los hilos finalicen
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);
    printf("Todas las tareas completadas, recursos optimizados.\n");
    return 0;
}

