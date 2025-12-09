#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[]) {
    int my_rank, comm_size;
    int local_sum = 0; // Suma local de cada proceso
    int global_sum = 0; // Suma total a ser calculada
    int n = 100; // Número total de elementos a sumar
    int *data = NULL; // Puntero para almacenar datos a sumar
    // Paso 1: Inicializar el entorno MPI
    MPI_Init(&argc, &argv);
    // Paso 2: Obtener el rango del proceso actual
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    // Paso 3: Obtener el número total de procesos
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    // Paso 4: Inicializar el arreglo de datos en el proceso raíz
    if (my_rank == 0) {
        data = (int*)malloc(n * sizeof(int)); // Asignar memoria para el arreglo
        for (int i = 0; i < n; i++) {
            data[i] = i + 1; // Inicializa el arreglo con valores del 1 al n
        }
    }
    // Paso 5: Dividir los datos entre los procesos
    int local_n = n / comm_size; // Número de elementos por proceso
    int *local_data = (int*)malloc(local_n * sizeof(int)); // Almacena datos locales
    // Paso 6: Distribuir los datos a todos los procesos
    MPI_Scatter(data, local_n, MPI_INT, local_data, local_n, MPI_INT, 0,
    MPI_COMM_WORLD);
    // Paso 7: Calcular la suma local en cada proceso
    for (int i = 0; i < local_n; i++) {
        local_sum += local_data[i]; // Suma los elementos locales
    }
    // Paso 8: Reducir las sumas locales a una suma global
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0,
    MPI_COMM_WORLD);
    // Paso 9: Imprimir el resultado en el proceso raíz
    if (my_rank == 0) {
        printf("Suma total: %d\n", global_sum); // Imprimir la suma total
        free(data); // Liberar memoria del arreglo original
    }
    // Paso 10: Liberar memoria local
    free(local_data); // Liberar memoria de los datos locales
    // Paso 11: Finalizar el entorno MPI
    MPI_Finalize();
    return 0;
}