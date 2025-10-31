#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
// Función que será ejecutada por los hilos
void* tarea(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 5; i++) {
        printf("Hilo %d ejecutando... Iteración %d\n", id, i);
    }
    pthread_exit(NULL);
}
int main() {
    pthread_t hilo1, hilo2; // Definir dos hilos
    int id1 = 1, id2 = 2;
    // Crear los hilos y asignarles la función tarea
    pthread_create(&hilo1, NULL, tarea, (void*)&id1);
    pthread_create(&hilo2, NULL, tarea, (void*)&id2);
    // Esperar a que los hilos terminen
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);
    printf("Hilos completados.\n");
    return 0;
}