#include <stdio.h>
#include <pthread.h>
int shared_value = 0; // Variable compartida
pthread_mutex_t lock; // Mutex para proteger la variable compartida
// Función que los hilos ejecutarán
void* increment_value(void* arg) {
    // Bloquear el mutex antes de modificar la variable compartida
    pthread_mutex_lock(&lock);
    // Sección crítica: modificar la variable compartida
    for (int i = 0; i < 100000; i++) {
        shared_value++;
    }
    // Desbloquear el mutex después de modificar la variable compartida
    pthread_mutex_unlock(&lock);
    return NULL;
}
int main() {
    pthread_t thread1, thread2;
    // Inicializar el mutex
    pthread_mutex_init(&lock, NULL);
    // Crear los hilos que ejecutan la función increment_value
    pthread_create(&thread1, NULL, increment_value, NULL);
    pthread_create(&thread2, NULL, increment_value, NULL);
    // Esperar a que los hilos terminen
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    // Imprimir el valor final de la variable compartida
    printf("Valor final de shared_value: %d\n", shared_value);
    // Destruir el mutex
    pthread_mutex_destroy(&lock);
    return 0;
}