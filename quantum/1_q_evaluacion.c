#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// Función que simula una tarea (como un algoritmo cuántico)
void simulatedTask() {
    // Simulación de una tarea que toma tiempo
    for (volatile long i = 0; i < 100000000; i++);
}
int main() {
    int numTests = 10; // Número de pruebas a realizar
    double totalTime = 0.0; // Tiempo total acumulado
    printf("Iniciando la evaluación del sistema...\n");
    for (int i = 0; i < numTests; i++) {
        // Iniciar el temporizador
        clock_t start = clock();
        // Ejecutar la tarea simulada
        simulatedTask();
        // Detener el temporizador
        clock_t end = clock();
        // Calcular el tiempo transcurrido
        double elapsedTime = (double)(end - start) / CLOCKS_PER_SEC;
        totalTime += elapsedTime;
        printf("Prueba %d: Tiempo transcurrido = %.6f segundos\n", i + 1,
        elapsedTime);
    }
    // Calcular el tiempo promedio
    double averageTime = totalTime / numTests;
    printf("Tiempo promedio de ejecución para %d pruebas: %.6f segundos\n",
    numTests, averageTime);
    return 0;
}