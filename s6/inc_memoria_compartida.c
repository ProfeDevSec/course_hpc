#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/wait.h>
int main() {
    // Paso 2: Crear una clave para la memoria compartida
    key_t key = ftok("shmfile", 65);
    // Paso 3: Crear o acceder a un segmento de memoria compartida
    int shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT);
    // Paso 4: Adjuntar el segmento de memoria compartida
    int *shared_value = (int *)shmat(shmid, NULL, 0);
    // Paso 5: Inicializar el valor en la memoria compartida
    *shared_value = 0;
    // Paso 6: Crear múltiples procesos para incrementar el valor
    if (fork() == 0) {
        // Proceso hijo 1
        for (int i = 0; i < 1000; i++) {
            (*shared_value)++;
        }
        shmdt(shared_value); // Desadjuntar la memoria compartida
        exit(0);
    } else if (fork() == 0) {
        // Proceso hijo 2
        for (int i = 0; i < 1000; i++) {
            (*shared_value)++;
        }
        shmdt(shared_value); // Desadjuntar la memoria compartida
        exit(0);
    }
    // Paso 7: Esperar a que los procesos hijos terminen
    wait(NULL); // Esperar a que el primer proceso hijo termine
    wait(NULL); // Esperar a que el segundo proceso hijo termine
    // Paso 8: Mostrar el valor final en la memoria compartida
    printf("Valor final en memoria compartida: %d\n", *shared_value);
    // Paso 9: Desadjuntar y eliminar el segmento de memoria compartida
    shmdt(shared_value);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}