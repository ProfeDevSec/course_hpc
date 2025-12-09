#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
// Función para realizar computación en un proceso
void perform_computation(int rank) {
    for (long i = 0; i < 1000000; i++) {
        double temp = (double)(rank + 1) * 3.14 * i; // Simula una carga de trabajo
        }
}
// Función para manejar errores
void handle_error(int error_code) {
    if (error_code != MPI_SUCCESS) {
        char error_string[100];
        int length_of_error_string;
        MPI_Error_string(error_code, error_string, &length_of_error_string);
        fprintf(stderr, "%s\n", error_string);
        MPI_Abort(MPI_COMM_WORLD, error_code);
    }
}
int main(int argc, char** argv) {
    int error_code = MPI_Init(&argc, &argv); // Inicializa el entorno MPI
    handle_error(error_code); // Maneja cualquier error en la inicialización
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtiene el identificador del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtiene el número total de procesos
    // Comprobación de la cantidad de procesos
    if (size < 2) {
        fprintf(stderr, "Se necesitan al menos dos procesos para este programa.\n");
        MPI_Abort(MPI_COMM_WORLD, 1); // Finaliza si no hay suficientes procesos
        }
    // Realiza la computación
    perform_computation(rank);
    // Proceso 0 envía un mensaje al Proceso 1
    int mensaje;
    int tag = 0; // Etiqueta del mensaje
    if (rank == 0) {
        mensaje = 123; // Mensaje a enviar
        printf("Proceso %d enviando mensaje %d al Proceso 1\n", rank,mensaje);
        error_code = MPI_Send(&mensaje, 1, MPI_INT, 1, tag,
        MPI_COMM_WORLD); // Envía el mensaje
        handle_error(error_code); // Maneja cualquier error en el envío
    }
    // Proceso 1 recibe el mensaje del Proceso 0
    else if (rank == 1) {
        error_code = MPI_Recv(&mensaje, 1, MPI_INT, 0, tag,
        MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Recibe el mensaje
        handle_error(error_code); // Maneja cualquier error en la recepción
        printf("Proceso %d recibió mensaje %d del Proceso 0\n", rank,mensaje);
    }
    // Finaliza el entorno MPI
    error_code = MPI_Finalize();
    handle_error(error_code); // Maneja cualquier error en la finalización
    return 0;
}