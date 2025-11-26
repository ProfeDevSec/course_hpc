/* locks_openmp.c */
#include <stdio.h>
#include <omp.h>

int main() {
    int shared_var = 0; // Variable compartida
    omp_lock_t lock; // Declaración del lock

    // Inicialización del lock
    omp_init_lock(&lock);

    #pragma omp parallel num_threads(4) 
    {
        int thread_id = omp_get_thread_num();

        // Establecer el lock para acceder a la sección crítica
        omp_set_lock(&lock);
        printf("Thread %d: Entrando a la sección crítica.\n", thread_id);

        // Modificación de la variable compartida
        shared_var += 1; 
        printf("Thread %d: shared_var = %d\n", thread_id, shared_var);

        // Salir de la sección crítica y liberar el lock
        printf("Thread %d: Saliendo de la sección crítica.\n", thread_id);
        omp_unset_lock(&lock);
    }

    // Destrucción del lock
    omp_destroy_lock(&lock);
    printf("Valor final de shared_var = %d\n", shared_var);

    return 0;
}