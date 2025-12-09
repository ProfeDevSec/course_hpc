#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0 // ID del proceso maestro
#define NUM_TASKS 10 // Número total de tareas a realizar

int main(int argc, char *argv[]) {
    int num_procs, rank; // Número de procesos y el rango del proceso
    int tasks[NUM_TASKS]; // Arreglo para almacenar las tareas
    int local_result = 0, global_result = 0; // Resultados locales y globales
    // Inicialización del entorno MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs); // Número total de procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Identificador del proceso
    if (rank == MASTER) {
        // Inicialización de las tareas en el proceso maestro
        for (int i = 0; i < NUM_TASKS; i++) {
            tasks[i] = i + 1; // Asignar tareas numeradas del 1 a NUM_TASKS
        }
    }
    // Distribuir las tareas a todos los procesos
    MPI_Scatter(tasks, NUM_TASKS / num_procs, MPI_INT,
    MPI_IN_PLACE, NUM_TASKS / num_procs, MPI_INT,
    MASTER, MPI_COMM_WORLD);
    // Procesar las tareas asignadas
    for (int i = 0; i < NUM_TASKS / num_procs; i++) {
        local_result += tasks[i]; // Sumar resultados locales
    }
    // Recolectar los resultados locales en el proceso maestro
    MPI_Reduce(&local_result, &global_result, 1, MPI_INT, MPI_SUM,
    MASTER, MPI_COMM_WORLD);

    // Mostrar el resultado global en el proceso maestro
    if (rank == MASTER) {
        printf("Resultado global (suma de todas las tareas): %d\n", global_result);
    }
    // Finalizar el entorno MPI
    MPI_Finalize();
    return 0;
}