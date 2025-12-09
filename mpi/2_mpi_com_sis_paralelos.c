#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // Inicializa el entorno MPI
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtiene el identificador del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtiene el número total de procesos
    if (size < 2) {
        fprintf(stderr, "Se necesitan al menos dos procesos para este programa.\n");
        MPI_Abort(MPI_COMM_WORLD, 1); // Finaliza si no hay suficientes procesos
        }
    int mensaje; // Variable para almacenar el mensaje
    // Proceso 0 envía un mensaje al Proceso 1
    if (rank == 0) {
        mensaje = 42; // Mensaje a enviar
        printf("Proceso %d enviando mensaje %d al Proceso 1\n", rank, mensaje);
        MPI_Send(&mensaje, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); // Envía el mensaje
        }
    // Proceso 1 recibe el mensaje del Proceso 0
    else if (rank == 1) {
        MPI_Recv(&mensaje, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE); // Recibe el mensaje
        printf("Proceso %d recibió mensaje %d del Proceso 0\n", rank, mensaje);
    }
    MPI_Finalize(); // Finaliza el entorno MPI
    return 0;
}