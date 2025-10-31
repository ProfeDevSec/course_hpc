#include <stdio.h> // Para funciones de entrada/salida como printf
#include <stdlib.h> // Para funciones generales como exit
#include <pthread.h> // Para el manejo de hilos

void* tarea1(void* arg) {
    for (int i = 0; i < 5; i++) {
        printf("Tarea 1 ejecutando... Iteración %d\n", i);
    }
    return NULL; // Los hilos siempre retornan un puntero genérico
}
void* tarea2(void* arg) {
    for (int i = 0; i < 5; i++) {
        printf("Tarea 2 ejecutando... Iteración %d\n", i);
    }
    return NULL; // Igual que en tarea1
}

int main() {
    pthread_t hilo1, hilo2; // Declarar las variables para almacenar los IDs de los hilos
     // Crear los hilos, pasando las funciones a ejecutar y un argumento (NULL en este caso)
     pthread_create(&hilo1, NULL, tarea1, NULL);
    pthread_create(&hilo2, NULL, tarea2, NULL);
    // Esperar a que los hilos terminen antes de que el programa principal continúe
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);
    printf("Tareas completadas.\n");
    return 0;
}

