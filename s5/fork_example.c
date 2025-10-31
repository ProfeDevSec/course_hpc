#include <stdio.h> // Para la función printf
#include <stdlib.h> // Para la función exit
#include <unistd.h> // Para fork() y getpid()
#include <sys/wait.h> // Para wait()

int main() {
   // int var = 0;
    pid_t pid; // Declaración de la variable para almacenar el PID
    // Crear un proceso hijo
    pid = fork();
    // Comprobar si fork() ha fallado
    if (pid < 0) {
        // Si fork() falla, imprime un mensaje de error y termina el programa
        fprintf(stderr, "Error al crear el proceso hijo.\n");
        return 1;
    }

    // Este bloque de código es ejecutado por el proceso hijo
    if (pid == 0) {
        // Proceso hijo
        printf("Proceso hijo (PID: %d) ejecutándose.\n", getpid()); // Obtener el PID del proceso hijo
         sleep(2); // Simula que el proceso hijo está realizando una tarea
        printf("Proceso hijo (PID: %d) finaliza.\n", getpid()); // El proceso hijo termina su ejecución
         }

    // Este bloque de código es ejecutado por el proceso padre
    else {
        // Proceso padre
        printf("Proceso padre (PID: %d) esperando al proceso hijo.\n", getpid()); // Obtener el PID del proceso padre
        wait(NULL); // El proceso padre espera a que el proceso hijo termine
        printf("Proceso padre (PID: %d) finaliza después de esperar al hijo.\n",
       getpid()); // El proceso padre termina
    }
    return 0;
}