/* scheduling_openmp.c */
#include <stdio.h>
#include <omp.h>

#define N 1000000 // Tamaño del arreglo

int main() {
    double a[N]; // Arreglo de N elementos
    double sum = 0.0; // Variable para la suma

    // Inicializar el arreglo con valores arbitrarios
    for (int i = 0; i < N; i++) {
        a[i] = i * 0.5;
    }

    omp_set_num_threads(4); // Usamos 4 hilos en todos los ejemplos

    // Scheduling estático con bloques de tamaño 10
    sum = 0.0;
    #pragma omp parallel for reduction(+:sum) schedule(static, 10)
    for (int i = 0; i < N; i++) {
        sum += a[i];
    }
    printf("Suma con static scheduling: %f\n", sum);

    // Scheduling dinámico con bloques de tamaño 10
    sum = 0.0;
    #pragma omp parallel for reduction(+:sum) schedule(dynamic, 10)
    for (int i = 0; i < N; i++) {
        sum += a[i];
    }
    printf("Suma con dynamic scheduling: %f\n", sum);

    // Scheduling guiado con bloques iniciales de tamaño 10
    sum = 0.0;
    #pragma omp parallel for reduction(+:sum) schedule(guided, 10)
    for (int i = 0; i < N; i++) {
        sum += a[i];
    }
    printf("Suma con guided scheduling: %f\n", sum);

    return 0;
}