#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h> // Para medir el tiempo con gettimeofday()
#define NUM_PROCESOS 3 // Definimos el número de procesos hijos
// Función para calcular el tiempo transcurrido en microsegundos
long calcular_tiempo_transcurrido(struct timeval inicio, struct timeval fin) {
    long segundos = fin.tv_sec - inicio.tv_sec;
    long microsegundos = fin.tv_usec - inicio.tv_usec;
    return (segundos * 1000000) + microsegundos; // Devuelve el tiempo en microsegundos
}

int main() {
     pid_t pid;
     int i;
     struct timeval tiempo_inicio, tiempo_fin; // Para medir el tiempo de ejecución
     // Marcar el tiempo de inicio antes de crear los procesos
     gettimeofday(&tiempo_inicio, NULL);

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
             printf("Proceso hijo %d (PID: %d) creado.\n", i + 1, getpid());

             // Simular una tarea que toma un tiempo diferente para cada hijo
             sleep(i + 1); // Cada proceso hijo duerme por 1, 2, y 3 segundos respectivamente

              printf("Proceso hijo %d (PID: %d) finaliza.\n", i + 1, getpid());
             exit(0); // Termina el proceso hijo correctamente
         }
     }
    // Proceso padre: espera la finalización de los hijos y evalúa el rendimiento
    for (i = 0; i < NUM_PROCESOS; i++) {
        int status;
        pid_t child_pid = wait(&status); // Espera la finalización de un proceso hijo
        if (WIFEXITED(status)) {
            printf("Proceso padre: el hijo con PID %d terminó correctamente. Estado de salida: %d\n", child_pid, WEXITSTATUS(status));
            } else {
                printf("Proceso padre: el hijo con PID %d no terminó correctamente.\n",child_pid);
            }
    }
    // Marcar el tiempo de finalización después de que todos los hijos terminen
    gettimeofday(&tiempo_fin, NULL);
    // Calcular el tiempo total transcurrido
    long tiempo_total = calcular_tiempo_transcurrido(tiempo_inicio, tiempo_fin);
    printf("Tiempo total de ejecución: %ld microsegundos.\n", tiempo_total);
    printf("Proceso padre (PID: %d) finaliza.\n", getpid());
    return 0;
}