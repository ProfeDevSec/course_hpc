/* mpi_nonblocking.c */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int send_data = rank + 100;
    int recv_data;
    MPI_Request request_send, request_recv;

    // Enviar y recibir de manera no bloqueante
    // Envía al siguiente vecino (en anillo), recibe del anterior
    int dest = (rank + 1) % size;
    int source = (rank - 1 + size) % size;

    printf("Proceso %d enviando a %d y recibiendo de %d\n", rank, dest, source);

    MPI_Isend(&send_data, 1, MPI_INT, dest, 0, MPI_COMM_WORLD, &request_send);
    MPI_Irecv(&recv_data, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &request_recv);

    // Esperar a que se completen
    MPI_Wait(&request_send, MPI_STATUS_IGNORE);
    MPI_Wait(&request_recv, MPI_STATUS_IGNORE);

    printf("Proceso %d recibió el dato: %d\n", rank, recv_data);

    MPI_Finalize();
    return 0;
}