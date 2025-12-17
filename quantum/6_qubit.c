#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
typedef struct {
    double amplitude_0; // Amplitud para el estado |0>
    double amplitude_1; // Amplitud para el estado |1>
} Qubit;
// Función para crear un qubit en el estado |0>
Qubit create_qubit() {
    Qubit q;
    q.amplitude_0 = 1.0; // |0> state
    q.amplitude_1 = 0.0; // |1> state
    return q;
}
// Función para aplicar una compuerta Hadamard al qubit
Qubit apply_hadamard(Qubit q) {
    Qubit new_q;
    double norm = sqrt(2); // Normalización para mantener la amplitud
    new_q.amplitude_0 = q.amplitude_0 / norm + q.amplitude_1 / norm;
    new_q.amplitude_1 = q.amplitude_0 / norm - q.amplitude_1 / norm;
    return new_q;
}
// Función para medir el qubit y obtener un resultado clásico
int measure(Qubit q) {
    double prob_0 = q.amplitude_0 * q.amplitude_0; // Probabilidad de medir |0>
    double prob_1 = q.amplitude_1 * q.amplitude_1; // Probabilidad de medir |1>
    double random = (double)rand() / RAND_MAX; // Número aleatorio entre 0 y 1
    printf("Random: %.2f\n", random);
    // Decidir el resultado basado en las probabilidades
    if (random < prob_0) {
        return 0; // Estado |0>
    } else {
        return 1; // Estado |1>
    }
}
int main() {
    // Inicialización del generador de números aleatorios
    srand(time(NULL));
    // Crear un qubit
    Qubit q = create_qubit();
    printf("Estado inicial del qubit: |0>\n");
    // Aplicar la compuerta Hadamard
    q = apply_hadamard(q);
    printf("Estado después de aplicar Hadamard: Amplitudes (|0>): %.2f, (|1>):%.2f\n",
    q.amplitude_0, q.amplitude_1);
    // Medir el qubit
    int result = measure(q);
    printf("Resultado de la medición: |%d>\n", result);
    return 0;
}