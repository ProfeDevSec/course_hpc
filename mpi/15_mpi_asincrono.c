#include <mpi.h>
#include <stdio.h>
#define TAG 0
int main(int argc, char **argv) {
    MPI_Init(&argc, &argv); // Inicializa el entorno MPI
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtiene el rango del  proceso (0 o 1)
    if (rank == 0) {
        int message = 456; // Mensaje a enviar
        MPI_Request request; // Variable para manejar la solicitud asíncrona
        printf("Proceso %d: Enviando mensaje asíncrono...\n", rank);
        MPI_Isend(&message, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD, &request); // Envío asíncrono
        printf("Proceso %d: Mensaje asíncrono enviado (continúa haciendo otras tareas).\n", rank);
        // Aquí puedes realizar otras tareas mientras el mensaje se envía
        // Simulamos una tarea adicional con un bucle
        for (int i = 0; i < 100000000; i++); // Tarea simulada
        // Espera a que se complete el envío
        MPI_Wait(&request, MPI_STATUS_IGNORE); // Finaliza la operación asíncrona
        } else if (rank == 1) {
            int message;
            MPI_Recv(&message, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE); // Recepción del mensaje
            printf("Proceso %d: Mensaje asíncrono recibido con valor %d.\n", rank, message);
        }
    MPI_Finalize(); // Finaliza el entorno MPI
    return 0; // Retorna 0 para indicar que el programa se ejecutó correctamente
}