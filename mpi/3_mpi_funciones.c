#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // Inicializa el entorno MPI
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtiene el identificador del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtiene el número total de procesos

    // Definición de variables
    int mensaje; // Variable para el mensaje
    int tag = 0; // Etiqueta del mensaje
    if (size < 2) {
        fprintf(stderr, "Se necesitan al menos dos procesos para este programa.\n");
        MPI_Abort(MPI_COMM_WORLD, 1); // Finaliza si no hay suficientes procesos
        }
    // Proceso 0 envía un mensaje al Proceso 1
    if (rank == 0) {
        mensaje = 123; // Mensaje a enviar
        printf("Proceso %d enviando mensaje %d al Proceso 1\n", rank, mensaje);
        MPI_Send(&mensaje, 1, MPI_INT, 1, tag, MPI_COMM_WORLD); // Envía el mensaje
        }
    // Proceso 1 recibe el mensaje del Proceso 0
    else if (rank == 1) {
        MPI_Recv(&mensaje, 1, MPI_INT, 0, tag, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE); // Recibe el mensaje
        printf("Proceso %d recibió mensaje %d del Proceso 0\n", rank, mensaje);
    }
    // Comprobación de un mensaje adicional: Proceso 1 envía una respuesta a Proceso 0
    if (rank == 1) {
        int respuesta = mensaje + 1; // Respuesta basada en el mensaje recibido
        printf("Proceso %d enviando respuesta %d al Proceso 0\n", rank, respuesta);
        MPI_Send(&respuesta, 1, MPI_INT, 0, tag, MPI_COMM_WORLD); // Envía la respuesta
        }
    else if (rank == 0) {
        MPI_Recv(&mensaje, 1, MPI_INT, 1, tag, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE); // Recibe la respuesta
        printf("Proceso %d recibió respuesta %d del Proceso 1\n", rank, mensaje);
    }
    MPI_Finalize(); // Finaliza el entorno MPI
    return 0;
}