#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void perform_computation(int rank, int size) {
    // Simula una carga de trabajo con un bucle
    for (long i = 0; i < 1000000; i++) {
        // Operaciones ficticias para simular el trabajo
        double temp = (double)(rank + 1) * 3.14 * i;
    }
}
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // Inicializa el entorno MPI
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtiene el identificador del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtiene el número total de procesos
    // Comenzamos a medir el tiempo
    double start_time, end_time;
    start_time = MPI_Wtime();
    // Llamamos a la función que realiza la computación
    perform_computation(rank, size);
    // Finalizamos la medición del tiempo
    end_time = MPI_Wtime();
    // Medimos el tiempo de ejecución
    double elapsed_time = end_time - start_time;
    // Solo el proceso 0 imprimirá el resultado
    if (rank == 0) {
        printf("Tiempo total de ejecución con %d procesos: %f segundos\n", size, elapsed_time);
    }
    MPI_Finalize(); // Finaliza el entorno MPI
    return 0;
}