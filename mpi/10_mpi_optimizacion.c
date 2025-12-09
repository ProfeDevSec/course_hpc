#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define DATA_SIZE 1000000 // Tamaño del dato a difundir
int main(int argc, char* argv[]) {
    int my_rank, comm_size;
    int *data = NULL; // Puntero para almacenar los datos
    // Paso 1: Inicializar el entorno MPI
    MPI_Init(&argc, &argv);
    // Paso 2: Obtener el rango del proceso actual
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    // Paso 3: Obtener el número total de procesos
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    // Paso 4: Asignar memoria para los datos solo en el proceso raíz
    if (my_rank == 0) {
        data = (int*)malloc(DATA_SIZE * sizeof(int)); // Asigna memoria para el array de datos
        for (int i = 0; i < DATA_SIZE; i++) {
            data[i] = i; // Inicializa los datos con valores
        }
    }
    // Paso 5: Realizar el Broadcast optimizado utilizando MPI_Bcast
    MPI_Bcast(data, DATA_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    // Paso 6: Utilizar los datos en cada proceso
    // (Simulación de procesamiento de datos)
    for (int i = 0; i < DATA_SIZE; i++) {
        data[i] += my_rank; // Cada proceso modifica los datos de acuerdo a su rango
    }
    // Paso 7: Imprimir una parte de los datos procesados por el proceso 0
    if (my_rank == 0) {
        printf("Proceso %d: datos procesados (primeros 10 valores): ", my_rank);
        for (int i = 0; i < 10; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
    }
    // Paso 8: Liberar la memoria en el proceso raíz
    if (my_rank == 0) {
        free(data); // Libera la memoria asignada
    }
    // Paso 9: Finalizar el entorno MPI
    MPI_Finalize();
    return 0;
}