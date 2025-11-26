/* suma_array_openmp.c */
#include <stdio.h>
#include <omp.h>

#define N 1000 // Tamaño del array

int main() {
    int i;
    double sum = 0.0;
    double array[N];

    // Inicialización del array con valores secuenciales
    for (i = 0; i < N; i++) {
        array[i] = i * 1.0; // Por ejemplo: 0, 1, 2, ..., N-1
    }

    // Paralelización de la suma de los elementos del array
    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < N; i++) {
        sum += array[i];
    }

    // Imprimir la suma total de los elementos del array
    printf("Suma total de los elementos del array: %f\n", sum);

    return 0;
}