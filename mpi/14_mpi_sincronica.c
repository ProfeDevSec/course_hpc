#include <mpi.h>
#include <stdio.h>

#define TAG 0

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv); // Inicializa el entorno MPI
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtiene el rango del proceso (0 o 1)
    if (rank == 0) {
        int message = 123; // Mensaje a enviar
        printf("Proceso %d: Enviando mensaje síncrono...\n", rank);
        MPI_Ssend(&message, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD); // Envío síncrono
        printf("Proceso %d: Mensaje síncrono enviado.\n", rank);
    } else if (rank == 1) {
        int message;
        MPI_Recv(&message, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE); // Recepción del mensaje
        printf("Proceso %d: Mensaje síncrono recibido con valor %d.\n", rank,
        message);
    }
    MPI_Finalize(); // Finaliza el entorno MPI
    return 0; // Retorna 0 para indicar que el programa se ejecutó correctamente
}


