#include <stdio.h>
#include <pthread.h>
int shared_value = 0;
pthread_mutex_t lock; // Declarar un mutex
void* increment_value(void* arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&lock); // Bloquear el acceso a la variable compartida
        shared_value++;
        pthread_mutex_unlock(&lock); // Desbloquear la variable compartida
    }
    return NULL;
}
int main() {
    pthread_t thread1, thread2;
    pthread_mutex_init(&lock, NULL); // Inicializar el mutex
    pthread_create(&thread1, NULL, increment_value, NULL);
    pthread_create(&thread2, NULL, increment_value, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_mutex_destroy(&lock); // Destruir el mutex
    printf("Valor final de shared_value: %d\n", shared_value);
    return 0;
}