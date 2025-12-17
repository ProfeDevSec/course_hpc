#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// Definición de la estructura para representar un qubit
typedef struct {
    double alpha; // Amplitud del estado |0>
    double beta; // Amplitud del estado |1>
} Qubit;
// Función para inicializar un qubit en el estado |0>
void initialize_qubit(Qubit *q) {
    q->alpha = 1.0; // Amplitud en |0>
    q->beta = 0.0; // Amplitud en |1>
}
// Función para aplicar la compuerta Hadamard a un qubit
void apply_hadamard(Qubit *q) {
    double new_alpha = (q->alpha + q->beta) / sqrt(2);
    double new_beta = (q->alpha - q->beta) / sqrt(2);
    q->alpha = new_alpha;
    q->beta = new_beta;
}
// Función para aplicar la compuerta Pauli-X (NOT)
void apply_pauli_x(Qubit *q) {
    double temp = q->alpha;
    q->alpha = q->beta;
    q->beta = temp;
}
// Función para mostrar el estado de un qubit
void display_qubit(Qubit q) {
    printf("Estado del qubit:\n");
    printf("|0>: %.2f, |1>: %.2f\n", q.alpha, q.beta);
}
int main() {
    // Crear un qubit
    Qubit q;
    // Inicializar el qubit
    initialize_qubit(&q);
    display_qubit(q);
    // Aplicar la compuerta Hadamard
    apply_hadamard(&q);
    printf("Después de aplicar Hadamard:\n");
    display_qubit(q);
    // Aplicar la compuerta Pauli-X
    apply_pauli_x(&q);
    printf("Después de aplicar Pauli-X:\n");
    display_qubit(q);
    return 0; // Fin del programa
}

