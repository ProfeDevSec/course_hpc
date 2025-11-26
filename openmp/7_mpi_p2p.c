/* mpi_p2p.c */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // Inicializa el entorno MPI
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Identificador del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Número total de procesos

    if (size < 2) {
        fprintf(stderr, "Se necesitan al menos dos procesos para este programa.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int mensaje;
    int tag = 0;

    // Proceso 0 envía un mensaje al Proceso 1
    if (rank == 0) {
        mensaje = 123;
        printf("Proceso %d enviando mensaje %d al Proceso 1\n", rank, mensaje);
        MPI_Send(&mensaje, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
    }
    // Proceso 1 recibe el mensaje del Proceso 0
    else if (rank == 1) {
        MPI_Recv(&mensaje, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Proceso %d recibió mensaje %d del Proceso 0\n", rank, mensaje);
    }

    // Comunicación adicional (Respuesta)
    if (rank == 1) {
        int respuesta = mensaje + 1;
        printf("Proceso %d enviando respuesta %d al Proceso 0\n", rank, respuesta);
        MPI_Send(&respuesta, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
    } 
    else if (rank == 0) {
        MPI_Recv(&mensaje, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Proceso %d recibió respuesta %d del Proceso 1\n", rank, mensaje);
    }

    MPI_Finalize(); // Finaliza el entorno MPI
    return 0;
}