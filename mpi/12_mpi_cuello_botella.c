#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Para usar sleep()
#define DATA_SIZE 1000000 // Tamaño del dato a enviar
int main(int argc, char* argv[]) {
    int my_rank, comm_size;
    int *data = NULL; // Puntero para almacenar los datos
    double start_time, end_time; // Variables para medir el tiempo
    // Paso 1: Inicializar el entorno MPI
    MPI_Init(&argc, &argv);
    // Paso 2: Obtener el rango del proceso actual
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    // Paso 3: Obtener el número total de procesos
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    // Paso 4: Inicializar el arreglo de datos en el proceso raíz
    if (my_rank == 0) {
        data = (int*)malloc(DATA_SIZE * sizeof(int)); // Asignar memoria para el arreglo
        for (int i = 0; i < DATA_SIZE; i++) {
            data[i] = i + 1; // Inicializa el arreglo con valores del 1 al DATA_SIZE
        }
    }
    // Paso 5: Simulación de un cálculo intensivo en el proceso raíz
    if (my_rank == 0) {
        printf("Proceso %d: realizando cálculos intensivos...\n", my_rank);
        sleep(5); // Simula un cálculo que tarda tiempo (5 segundos)
        printf("Proceso %d: cálculos completados, enviando datos...\n", my_rank);
    }
    // Paso 6: Medir el tiempo de inicio para la comunicación
    MPI_Barrier(MPI_COMM_WORLD); // Sincroniza todos los procesos
    start_time = MPI_Wtime(); // Captura el tiempo inicial
    // Paso 7: Enviar los datos al resto de los procesos
    MPI_Bcast(data, DATA_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    // Paso 8: Medir el tiempo de finalización para la comunicación
    end_time = MPI_Wtime(); // Captura el tiempo final
    // Paso 9: Cada proceso imprime el tiempo de comunicación
    printf("Proceso %d: tiempo de comunicación: %f segundos\n", my_rank,
    end_time - start_time);
    // Paso 10: Liberar memoria en el proceso raíz
    if (my_rank == 0) {
        free(data); // Liberar memoria del arreglo original
    }
    // Paso 11: Finalizar el entorno MPI
    MPI_Finalize();
    return 0;
}