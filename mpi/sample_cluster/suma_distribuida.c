#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000000  // Tamaño del problema: sumar 1..N

int main(int argc, char *argv[]) {
    int rank, size;
    int name_len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // ID de este proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Nº total de procesos

    MPI_Get_processor_name(processor_name, &name_len);

    // Comprobamos que N sea divisible por size para simplificar el ejemplo
    if (N % size != 0) {
        if (rank == 0) {
            fprintf(stderr,
                    "Error: N=%d no es divisible por size=%d\n",
                    N, size);
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int local_n = N / size;   // Cuántos elementos le tocan a cada proceso
    int start = rank * local_n + 1;
    int end   = start + local_n - 1;

    // Cada proceso calcula la suma parcial de su rango [start, end]
    long long local_sum = 0;
    for (int i = start; i <= end; i++) {
        local_sum += i;
    }

    // Mostramos por pantalla quién es quién
    printf("Proceso %d de %d corriendo en %s: rango [%d, %d], suma local = %lld\n",
           rank, size, processor_name, start, end, local_sum);

    // Reducimos todas las sumas locales en el proceso 0
    long long global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG_LONG,
               MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Valor teórico de la suma 1..N = N*(N+1)/2
        long long expected = (long long)N * (N + 1) / 2;
        printf("\nSuma global calculada = %lld\n", global_sum);
        printf("Suma esperada          = %lld\n", expected);
    }

    MPI_Finalize();
    return 0;
}
