#include <stdio.h> // Para printf
#include <stdlib.h> // Para exit
#include <unistd.h> // Para fork(), sleep(), getpid()
#include <sys/wait.h> // Para wait()

#define NUM_PROCESOS 3 // Definimos el número de procesos hijos
int main() {
    pid_t pid;
    int i;
    // Crear múltiples procesos hijos
    for (i = 0; i < NUM_PROCESOS; i++) {
        pid = fork(); // Crear un nuevo proceso hijo
        if (pid < 0) {
            // Si fork() falla, imprime un mensaje de error y termina el programa
            fprintf(stderr, "Error al crear el proceso hijo.\n");
            return 1;
        }
        if (pid == 0) {
            // Código que ejecuta el proceso hijo
            printf("Proceso hijo %d (PID: %d) creado, ejecutándose de manera concurrente.\n", i+1, getpid());
            // Simulamos que el proceso hijo está realizando una tarea
            sleep(i + 1); // Cada hijo "trabaja"
            printf("Proceso hijo %d (PID: %d) finaliza después de %d segundos.\n", i+1, getpid(), i + 1);
            exit(0); // Termina el proceso hijo
        }
    }
    // TODO Código incompleto
}