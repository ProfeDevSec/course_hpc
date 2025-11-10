#include <stdio.h> // Para la función printf
#include <stdlib.h> // Para la función exit
#include <unistd.h> // Para fork(), sleep(), y getpid()
#include <sys/wait.h> // Para wait()
int main() {
    pid_t pid; // Declaración de la variable para almacenar el PID
    pid_t ppid;
    // Crear un proceso hijo
    ppid = getpid();
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
        printf("Mi PADRE ES (PID: %d) ejecutándose.\n", ppid);
         sleep(5); // Simula que el proceso hijo está realizando una tarea durante 5 segundos
         printf("Proceso hijo (PID: %d) ha terminado su tarea y finaliza.\n",getpid());
         pid = fork();
         if (pid == 0) {
             getpid();
           }else{
             //
        }
        getpid(); // El proceso hijo termina su ejecución
    }
    // Este bloque de código es ejecutado por el proceso padre
    else {
        // Proceso padre
        printf("Proceso padre (PID: %d) esperando al proceso hijo.\n",
       getpid()); // Obtener el PID del proceso padre
        // Espera a que el proceso hijo termine
        int status; // Variable para almacenar el estado del proceso hijo
        wait(&status); // El proceso padre espera a que el proceso hijo termine
        if (WIFEXITED(status)) { // Comprobar si el hijo terminó correctamente
            printf("Proceso hijo terminó con éxito. Estado de salida: %d\n",
           WEXITSTATUS(status));
        } else {
            printf("El proceso hijo no terminó correctamente.\n");
        }
        printf("Proceso padre (PID: %d) finaliza después de esperar al hijo.\n",
       getpid()); // El proceso padre termina
    }
    return 0;
}