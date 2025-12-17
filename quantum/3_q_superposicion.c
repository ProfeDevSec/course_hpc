#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// Estructura para representar un qubit
typedef struct {
    double amplitude0; // Amplitud para el estado |0>
    double amplitude1; // Amplitud para el estado |1>
} Qubit;
// Función para crear un qubit en superposición
Qubit createSuperposition(double amplitude0, double amplitude1) {
    Qubit qubit;
    double norm = sqrt(amplitude0 * amplitude0 + amplitude1 * amplitude1); // Normalizar las amplitudes
    qubit.amplitude0 = amplitude0 / norm;
    qubit.amplitude1 = amplitude1 / norm;
    return qubit;
}
// Función para medir el estado de un qubit
int measureQubit(Qubit qubit) {
    double randNum = (double)rand() / RAND_MAX; // Número aleatorio entre 0 y 1
    double probability0 = qubit.amplitude0 * qubit.amplitude0; // Probabilidad de |0>
    // Medir el qubit basado en las probabilidades
    if (randNum < probability0) {
        return 0; // Estado |0>
    } else {
        return 1; // Estado |1>
    }
}
// Función para mostrar el estado de un qubit
void printQubit(Qubit q) {
    printf("Qubit en superposición: |0> con amplitud: %.2f, |1> con amplitud:%.2f\n", q.amplitude0, q.amplitude1);
}
int main() {
    srand(time(NULL)); // Inicializar la semilla aleatoria
    // Crear un qubit en superposición
    double amplitude0 = 1.0; // Amplitud para |0>
    double amplitude1 = 1.0; // Amplitud para |1>
    Qubit qubit = createSuperposition(amplitude0, amplitude1);
    // Mostrar el estado del qubit
    printQubit(qubit);
    // Medir el qubit varias veces y mostrar los resultados
    int numMeasurements = 10;
    printf("\nResultados de las mediciones:\n");
    for (int i = 0; i < numMeasurements; i++) {
        int result = measureQubit(qubit);
        printf("Medición %d: Estado = |%d>\n", i + 1, result);
    }
    return 0;
}