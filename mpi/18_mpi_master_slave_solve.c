#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0      // ID del proceso maestro
#define NUM_TASKS 10  // Número total de tareas a realizar

int main(int argc, char *argv[]) {
    int num_procs, rank;
    int tasks[NUM_TASKS];             // Solo se usará realmente en el maestro
    int local_result = 0, global_result = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Comprobamos que NUM_TASKS es divisible por num_procs
    if (NUM_TASKS % num_procs != 0) {
        if (rank == MASTER) {
            fprintf(stderr, "NUM_TASKS (%d) no es divisible por num_procs (%d)\n",
                    NUM_TASKS, num_procs);
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int chunk = NUM_TASKS / num_procs;

    if (rank == MASTER) {
        // Inicialización de las tareas en el proceso maestro
        for (int i = 0; i < NUM_TASKS; i++) {
            tasks[i] = i + 1; // Tareas 1..NUM_TASKS
        }
    }

    // Buffer local para las tareas de cada proceso
    int *local_tasks = (int *)malloc(chunk * sizeof(int));
    if (local_tasks == NULL) {
        fprintf(stderr, "Proceso %d: error al reservar memoria\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 2);
    }

    // Distribuir las tareas a todos los procesos
    MPI_Scatter(tasks,       // sendbuf (solo válido en MASTER)
                chunk, MPI_INT,
                local_tasks, // recvbuf en TODOS los procesos
                chunk, MPI_INT,
                MASTER, MPI_COMM_WORLD);

    // Procesar las tareas asignadas
    for (int i = 0; i < chunk; i++) {
        local_result += local_tasks[i];
    }

    // Recolectar los resultados locales en el proceso maestro
    MPI_Reduce(&local_result, &global_result,
               1, MPI_INT, MPI_SUM,
               MASTER, MPI_COMM_WORLD);

    if (rank == MASTER) {
        printf("Resultado global (suma de todas las tareas): %d\n", global_result);
        // Para NUM_TASKS=10, debería dar 55
    }

    free(local_tasks);
    MPI_Finalize();
    return 0;
}
