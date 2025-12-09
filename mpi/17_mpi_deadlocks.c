#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // Inicializar el entorno MPI
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtener el rango del   proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtener el número de  procesos
    if (size < 2) {
        printf("Este programa necesita al menos 2 procesos para ejecutarse.\n");
        MPI_Abort(MPI_COMM_WORLD, 1); // Terminar si no hay suficientes  procesos
        }
    int send_data = rank + 100; // Cada proceso envía un número diferente
    int recv_data = -1;
    if (rank == 0) {
        // Proceso 0 intenta enviar a Proceso 1 y luego recibir de él
        MPI_Send(&send_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);
        printf("Proceso 0 recibió %d de Proceso 1\n", recv_data);
    } else if (rank == 1) {
        // Proceso 1 intenta enviar a Proceso 0 y luego recibir de él
        MPI_Send(&send_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);
        printf("Proceso 1 recibió %d de Proceso 0\n", recv_data);
    }
    MPI_Finalize(); // Finalizar el entorno MPI
    return 0; // Retornar 0
}


