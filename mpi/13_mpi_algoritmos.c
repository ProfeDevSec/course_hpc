#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define VECTOR_SIZE 1000000 // Tamaño de los vectores
int main(int argc, char* argv[]) {
    int my_rank, comm_size;
    double *vector_a = NULL; // Puntero para el primer vector
    double *vector_b = NULL; // Puntero para el segundo vector
    double *local_a = NULL; // Puntero para la parte local del vector A
    double *local_b = NULL; // Puntero para la parte local del vector B
    double local_sum = 0.0; // Suma local del producto escalar
    double global_sum = 0.0; // Suma total del producto escalar
    // Paso 1: Inicializar el entorno MPI
    MPI_Init(&argc, &argv);
    // Paso 2: Obtener el rango del proceso actual
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    // Paso 3: Obtener el número total de procesos
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    // Paso 4: Inicializar los vectores en el proceso raíz
    if (my_rank == 0) {
        vector_a = (double*)malloc(VECTOR_SIZE * sizeof(double));
        vector_b = (double*)malloc(VECTOR_SIZE * sizeof(double));
        // Inicializar los vectores con valores
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vector_a[i] = i + 1; // Vector A: 1, 2, ..., VECTOR_SIZE
            vector_b[i] = VECTOR_SIZE - i; // Vector B: VECTOR_SIZE, VECTOR_SIZE-1, ..., 1
            }
    }
    // Paso 5: Dividir el trabajo entre los procesos
    int local_n = VECTOR_SIZE / comm_size; // Número de elementos por proceso
    local_a = (double*)malloc(local_n * sizeof(double));
    local_b = (double*)malloc(local_n * sizeof(double));
    // Paso 6: Distribuir los vectores a todos los procesos
    MPI_Scatter(vector_a, local_n, MPI_DOUBLE, local_a, local_n,
    MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(vector_b, local_n, MPI_DOUBLE, local_b, local_n,
    MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // Paso 7: Calcular el producto escalar local
    for (int i = 0; i < local_n; i++) {
        local_sum += local_a[i] * local_b[i]; // Suma del producto escalar local
    }
    // Paso 8: Reducir las sumas locales a una suma global
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0,
    MPI_COMM_WORLD);
    // Paso 9: Imprimir el resultado en el proceso raíz
    if (my_rank == 0) {
        printf("El producto escalar es: %f\n", global_sum);
        free(vector_a); // Liberar memoria del vector A
        free(vector_b); // Liberar memoria del vector B
    }
    // Paso 10: Liberar memoria local
    free(local_a); // Liberar memoria del vector local A
    free(local_b); // Liberar memoria del vector local B
    // Paso 11: Finalizar el entorno MPI
    MPI_Finalize();
    return 0;
}