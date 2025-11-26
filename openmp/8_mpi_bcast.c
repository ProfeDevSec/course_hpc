/* mpi_bcast.c */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int my_rank, comm_size;
    int data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    // Definir el proceso raíz y los datos
    if (my_rank == 0) {
        data = 100;
        printf("Proceso %d: enviando valor %d a todos los procesos.\n", my_rank, data);
    }

    // Realizar el Broadcast desde el proceso raíz (0)
    MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Todos los procesos imprimen el valor recibido
    printf("Proceso %d: recibió valor %d\n", my_rank, data);

    MPI_Finalize();
    return 0;
}