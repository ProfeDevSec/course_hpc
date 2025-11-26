/* mpi_scatter_gather.c */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int numtasks, rank;
    const int root = 0;
    int send_data[16]; // Datos originales (solo en root)
    int recv_data[4];  // Buffer local para recibir 4 elementos
    int gathered_data[16]; // Buffer final

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // Asumir 4 procesos para este ejemplo
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Inicialización de datos en el proceso raíz
    if (rank == root) {
        for (int i = 0; i < 16; i++) {
            send_data[i] = i;
        }
        printf("Proceso raíz envía datos...\n");
    }

    // Distribuir 4 elementos a cada proceso (asumiendo 4 procesos)
    MPI_Scatter(send_data, 4, MPI_INT, recv_data, 4, MPI_INT, root, MPI_COMM_WORLD);

    printf("Proceso %d recibió sus datos.\n", rank);

    // Procesamiento simple: multiplicar por 2
    for (int i = 0; i < 4; i++) {
        recv_data[i] *= 2;
    }

    // Recolectar resultados en el proceso raíz
    MPI_Gather(recv_data, 4, MPI_INT, gathered_data, 4, MPI_INT, root, MPI_COMM_WORLD);

    // Imprimir resultados finales
    if (rank == root) {
        printf("Datos recolectados y procesados en raíz: ");
        for (int i = 0; i < 16; i++) {
            printf("%d ", gathered_data[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}