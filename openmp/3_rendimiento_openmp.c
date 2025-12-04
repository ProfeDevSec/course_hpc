/* rendimiento_openmp.c */
#include <stdio.h>
#include <omp.h>

#define N 1000000 // Tamaño del array

int main() {
    int i;
    double array[N], sum = 0.0;
    double start_time, end_time;

    // Inicialización del array con valores secuenciales
    for (i = 0; i < N; i++) {
        array[i] = i * 1.0;
    }

    // Calcular la suma en serie
    start_time = omp_get_wtime(); // Iniciar cronómetro
    for (i = 0; i < N; i++) {
        sum += array[i];
    }
    end_time = omp_get_wtime(); // Finalizar cronómetro
    printf("Tiempo en serie: %f segundos\n", end_time - start_time);
    printf("Suma en serie: %f\n", sum);

    // Reiniciar la suma
    sum = 0.0;

    // Calcular la suma en paralelo
    start_time = omp_get_wtime(); // Iniciar cronómetro
    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < N; i++) {
        sum += array[i];
    }
    end_time = omp_get_wtime(); // Finalizar cronómetro
    printf("Tiempo en paralelo: %f segundos\n", end_time - start_time);
    printf("Suma en paralelo: %f\n", sum);

    // Establecer el número de hilos manualmente (ejemplo 4 hilos)
    omp_set_num_threads(4);
    sum = 0.0;

    start_time = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < N; i++) {
        sum += array[i];
    }
    end_time = omp_get_wtime();
    printf("Tiempo en paralelo con 4 hilos: %f segundos\n", end_time - start_time);
    printf("Suma con 4 hilos: %f\n", sum);

    // Establecer el número de hilos manualmente (ejemplo 4 hilos)
    omp_set_num_threads(2);
    sum = 0.0;

    start_time = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < N; i++) {
        sum += array[i];
    }
    end_time = omp_get_wtime();
    printf("Tiempo en paralelo con 2 hilos: %f segundos\n", end_time - start_time);
    printf("Suma con 2 hilos: %f\n", sum);

    return 0;
}