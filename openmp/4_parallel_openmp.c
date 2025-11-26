#include <stdio.h>
#include <omp.h>
int main() {
    int num_threads = 4; // Número de hilos
    omp_set_num_threads(num_threads);  // Fijar el número de hilos [12]
    printf("Ejemplo de directivas de OpenMP con %d hilos\n", num_threads);
    // Directiva 'parallel' para crear una región paralela
#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();  // Obtener el número del hilo
        printf("Hola desde el hilo %d\n", thread_id);
    }
    int sum = 0;
    // Directiva 'parallel for' para paralelizar un bucle
#pragma omp parallel for
    for (int i = 0; i < 10; i++) {
        printf("Hilo %d procesando iteración %d\n", omp_get_thread_num(), i);
    }
    // Directiva 'parallel for' con 'critical' para sincronización
#pragma omp parallel for [13]
    for (int i = 0; i < 10; i++) {
#pragma omp critical
        {
            sum += i;  // Modificar la variable compartida de forma sincronizada
            printf("Hilo %d sumando %d, total = %d\n", omp_get_thread_num(), i, sum);
        }
    }
    printf("Suma total con critical: %d\n", sum);
    // Directiva 'single' para que solo un hilo ejecute una sección de código [14]
#pragma omp parallel
    {
#pragma omp single
        {
            printf("Esta sección la ejecuta el hilo %d\n", omp_get_thread_num());
        }
    }
    // Directiva 'barrier' para sincronizar los hilos
#pragma omp parallel
{
    printf("Hilo %d antes de la barrera\n", omp_get_thread_num());
#pragma omp barrier  // Sincronización de todos los hilos [15]
    printf("Hilo %d después de la barrera\n", omp_get_thread_num());
}
    return 0;
}