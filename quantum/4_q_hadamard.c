#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
// Estructura para representar un qubit
typedef struct {
    double amplitude0; // Amplitud para el estado |0>
    double amplitude1; // Amplitud para el estado |1>
} Qubit;
// Función para inicializar un qubit en el estado |0>
Qubit initializeQubit() {
    Qubit qubit;
    qubit.amplitude0 = 1.0; // Estado |0> tiene amplitud 1
    qubit.amplitude1 = 0.0; // Estado |1> tiene amplitud 0
    return qubit;
}
// Función para aplicar la compuerta Hadamard
Qubit applyHadamard(Qubit qubit) {
    Qubit result;
    result.amplitude0 = (qubit.amplitude0 + qubit.amplitude1) / sqrt(2); // Nueva amplitud para |0>
    result.amplitude1 = (qubit.amplitude0 - qubit.amplitude1) / sqrt(2); // Nueva amplitud para |1>
    return result;
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
    printf("Qubit: |0> con amplitud: %.2f, |1> con amplitud: %.2f\n", q.amplitude0,
    q.amplitude1);
}
int main() {
    srand(time(NULL)); // Inicializar la semilla aleatoria
    // Inicializar el qubit en el estado |0>
    Qubit qubit = initializeQubit();
    printf("Estado inicial:\n");
    printQubit(qubit);
    // Aplicar la compuerta Hadamard
    qubit = applyHadamard(qubit);
    printf("\nDespués de aplicar la compuerta Hadamard:\n");
    printQubit(qubit);
    // Medir el qubit y mostrar el resultado
    int result = measureQubit(qubit);
    printf("\nResultado de la medición: Estado = |%d>\n", result);
    return 0;
}