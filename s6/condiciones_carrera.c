#include <stdio.h>
#include <pthread.h> // Para hilos
#include <unistd.h> // Para sleep
int shared_value = 0; // Variable compartida
// Función que los hilos ejecutarán
void* increment_value(void* arg) {
    for (int i = 0; i < 100000; i++) {
        shared_value++; // Modificación concurrente
    }
    return NULL;
}
int main() {
    pthread_t thread1, thread2;
    // Crear dos hilos
    pthread_create(&thread1, NULL, increment_value, NULL);
    pthread_create(&thread2, NULL, increment_value, NULL);
    // Esperar a que los hilos terminen
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    // Imprimir el valor final
    printf("Valor final de shared_value: %d\n", shared_value);
    return 0;
}