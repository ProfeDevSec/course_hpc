#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
void perform_computation(int rank) {
    // Simula una carga de trabajo
    for (long i = 0; i < 1000000; i++) {
        double temp = (double)(rank + 1) * 3.14 * i;
    }
}
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // Inicializa el entorno MPI
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtiene el identificador del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtiene el número total de procesos
    double start_time, end_time;
    double total_time = 0.0;
    // Tiempo de computación
    start_time = MPI_Wtime(); // Comienza a medir el tiempo
    perform_computation(rank); // Realiza la computación
    end_time = MPI_Wtime(); // Finaliza la medición del tiempo
    total_time += (end_time - start_time); // Acumula el tiempo total
    // Tiempo de envío y recepción
    int mensaje;
    int tag = 0; // Etiqueta del mensaje
    if (rank == 0) {
        mensaje = 123; // Mensaje a enviar
        printf("Proceso %d enviando mensaje %d al Proceso 1\n", rank, mensaje);
        // Medir el tiempo de envío
        start_time = MPI_Wtime();
        MPI_Send(&mensaje, 1, MPI_INT, 1, tag, MPI_COMM_WORLD); // Envía el mensaje
        end_time = MPI_Wtime();
        double send_time = end_time - start_time; // Tiempo de envío
        printf("Tiempo de envío: %f segundos\n", send_time);
    } else if (rank == 1) {
        // Medir el tiempo de recepción
        start_time = MPI_Wtime();
        MPI_Recv(&mensaje, 1, MPI_INT, 0, tag, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE); // Recibe el mensaje
        end_time = MPI_Wtime();
        double recv_time = end_time - start_time; // Tiempo de recepción
        printf("Proceso %d recibió mensaje %d del Proceso 0\n", rank, mensaje);
        printf("Tiempo de recepción: %f segundos\n", recv_time);
    }
    // Calcular y mostrar la eficiencia
    double ideal_time = 1.0; // Tiempo ideal (en segundos) para este ejemplo
    double efficiency = ideal_time / total_time; // Cálculo de eficiencia
    // Solo el proceso 0 imprime la eficiencia
    if (rank == 0) {
        printf("Tiempo total de ejecución: %f segundos\n", total_time);
        printf("Eficiencia: %f\n", efficiency);
    }
    MPI_Finalize(); // Finaliza el entorno MPI
    return 0;
}