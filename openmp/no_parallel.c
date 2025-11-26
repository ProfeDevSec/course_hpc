#include <stdio.h>
#include <omp.h>  // Biblioteca de OpenMP [26]
#define N 1000  // Tamaño del array
int main() {
    int i;
    double array[N];
    // Inicialización del array con valores secuenciales
    for (i = 0; i < N; i++) {
        array[i] = i * 1.0;  // Por ejemplo: 0, 1, 2, ..., N-1
    }
    // Bucle con dependencia de datos: array[i] depende de array[i-1]
    for (i = 1; i < N; i++) {
        array[i] = array[i-1] + i; [26]
        }
    // Impresión de los primeros 10 elementos para verificar el resultado
    printf("Primeros 10 elementos del array:\n");
    for (i = 0; i < 10; i++) {
        printf("array[%d] = %f\n", i, array[i]); [27]
        }
    return 0;
}