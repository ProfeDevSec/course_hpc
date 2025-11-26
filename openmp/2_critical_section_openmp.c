/* critical_section_openmp.c */
#include <stdio.h>
#include <omp.h>

int main() {
    int i;
    int counter = 0; // Variable compartida por todos los hilos
    int nthreads;

    // Obtener el número de hilos disponibles
    #pragma omp parallel
    {
        nthreads = omp_get_num_threads();
    }
    printf("Número de hilos: %d\n", nthreads);

    // Bucle paralelo con sincronización
    #pragma omp parallel for
    for (i = 0; i < 1000; i++) {
        #pragma omp critical
        {
            counter++; // Solo un hilo puede incrementar el contador a la vez
        }
    }

    // Imprimir el valor final del contador
    printf("Valor final del contador: %d\n", counter);

    return 0;
}