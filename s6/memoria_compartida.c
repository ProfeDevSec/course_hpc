#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h> // Para memoria compartida
#include <sys/ipc.h> // Para claves de IPC
#include <unistd.h> // Para fork
#include <sys/wait.h> // Para esperar a los procesos

int main() {
    // Paso 2: Crear una clave para identificar la memoria compartida
    key_t key = ftok("shmfile", 65);
    // Paso 3: Crear o acceder a un segmento de memoria compartida
    int shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT);
    // Paso 4: Adjuntar el segmento de memoria compartida al espacio de direcciones del proceso
    int *shared_value = (int *) shmat(shmid, NULL, 0);
    // Paso 5: Inicializar el valor en la memoria compartida
    *shared_value = 0;
    // Paso 6: Crear un proceso hijo que modifique el valor en la memoria compartida
    if (fork() == 0) {
        // Proceso hijo
        *shared_value += 5;
        printf("Proceso hijo: valor = %d\n", *shared_value);
        shmdt(shared_value); // Desadjuntar el segmento de memoria compartida
        exit(0); // Terminar el proceso hijo
    } else {
        // Proceso padre
        wait(NULL); // Esperar al proceso hijo
        printf("Proceso padre: valor = %d\n", *shared_value);
    }
    // Paso 7: Desadjuntar y eliminar el segmento de memoria compartida
    shmdt(shared_value);
    shmctl(shmid, IPC_RMID, NULL); // Eliminar el segmento de memoria compartida
    return 0;
}