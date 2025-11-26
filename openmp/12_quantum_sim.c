/* quantum_sim.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_QUBITS 2

// Estructura Qubit (Amplitudes para |0> y |1>)
typedef struct {
    double alpha; // |0>
    double beta;  // |1>
} Qubit;

// Inicializar qubit en |0>
void initialize_qubit(Qubit *q) {
    q->alpha = 1.0;
    q->beta = 0.0;
}

// Compuerta Hadamard: Crea superposición
void apply_hadamard(Qubit *q) {
    double new_alpha = (q->alpha + q->beta) / sqrt(2);
    double new_beta = (q->alpha - q->beta) / sqrt(2);
    q->alpha = new_alpha;
    q->beta = new_beta;
}

// Compuerta CNOT: Invierte target si control está en |1>
void apply_cnot(Qubit *control, Qubit *target) {
    if (control->beta != 0.0) { // Simplificación de simulación
        double temp = target->alpha;
        target->alpha = target->beta;
        target->beta = temp;
    }
}

// Medir Qubit (Colapso de función de onda probabilística)
int measure_qubit(Qubit q) {
    double random_value = (double)rand() / RAND_MAX;
    if (random_value < q.alpha * q.alpha) {
        return 0;
    } else {
        return 1;
    }
}

int main() {
    srand(time(NULL));
    Qubit qubits[NUM_QUBITS];

    // Inicializar
    for (int i = 0; i < NUM_QUBITS; i++) initialize_qubit(&qubits[i]);

    printf("Aplicando Hadamard al Qubit 0...\n");
    apply_hadamard(&qubits[0]);

    printf("Aplicando CNOT (Control: Q0, Target: Q1)...\n");
    apply_cnot(&qubits[0], &qubits[1]);

    printf("Resultados de medición:\n");
    for (int i = 0; i < NUM_QUBITS; i++) {
        int res = measure_qubit(qubits[i]);
        printf("Qubit %d medido: |%d>\n", i, res);
    }

    return 0;
}