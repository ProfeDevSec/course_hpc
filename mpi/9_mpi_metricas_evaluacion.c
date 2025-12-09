#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, char* argv[]) {
    int my_rank, comm_size;
    int data;
    double start_time, end_time; // Variables para medir el tiempo
    // Paso 1: Inicializar el entorno MPI
    MPI_Init(&argc, &argv);
    // Paso 2: Obtener el rango del proceso actual
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    // Paso 3: Obtener el número total de procesos
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    // Paso 4: Definir el proceso raíz y los datos que se van a enviar
    if (my_rank == 0) {
        data = 100; // El proceso raíz inicializa el valor
        printf("Proceso %d: enviando valor %d a todos los procesos.\n", my_rank,data);
    }
    // Paso 5: Iniciar la medición del tiempo
    MPI_Barrier(MPI_COMM_WORLD); // Sincroniza todos los procesos
    start_time = MPI_Wtime(); // Captura el tiempo inicial
    // Paso 6: Realizar el Broadcast desde el proceso raíz a todos los demás procesos
    MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // Paso 7: Finalizar la medición del tiempo
    end_time = MPI_Wtime(); // Captura el tiempo final
    // Paso 8: Todos los procesos imprimen el valor recibido y el tiempo tomado
    printf("Proceso %d: recibió valor %d\n", my_rank, data);
    printf("Proceso %d: tiempo de ejecución para MPI_Bcast: %f segundos\n",
    my_rank, end_time - start_time);
    // Paso 9: Finalizar el entorno MPI
    MPI_Finalize();
    return 0;
}