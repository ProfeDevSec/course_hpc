/* private_shared_openmp.c */
#include <stdio.h>
#include <omp.h>

int main() {
    int shared_var = 100; // Variable compartida por todos los hilos
    int private_var; // Variable que será privada

    // Inicia una región paralela
    #pragma omp parallel private(private_var) shared(shared_var)
    {
        // Cada hilo tiene su propia copia de private_var
        private_var = omp_get_thread_num(); 
        printf("Hilo %d: Valor inicial de private_var = %d\n", private_var, private_var);

        // Modificar la variable privada
        private_var += 10;
        printf("Hilo %d: private_var después de la modificación = %d\n", 
               omp_get_thread_num(), private_var);

        // Sección crítica para modificar la variable compartida
        #pragma omp critical
        {
            shared_var += 1; // Cada hilo modifica la variable compartida
        }
    }

    // Imprime el valor final de la variable compartida
    printf("Valor final de shared_var = %d\n", shared_var);

    return 0;
}