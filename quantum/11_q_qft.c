#include <stdio.h>
#include <math.h>
#define NUM_QUBITS 3 // Número de qubits
// Función para inicializar el estado cuántico
void initialize_state(double state[NUM_QUBITS][2]) {
    // Inicializar el estado |0>
    state[0][0] = 1.0; // Amplitud de |0>
    state[0][1] = 0.0; // Amplitud de |1>
    for (int i = 1; i < (1 << NUM_QUBITS); i++) {
        state[i][0] = 0.0; // Amplitudes de otros estados
        state[i][1] = 0.0;
    }
}
// Función para aplicar la QFT
void apply_qft(double state[NUM_QUBITS][2]) {
    int n = 1 << NUM_QUBITS; // Número total de estados
    for (int j = 0; j < NUM_QUBITS; j++) {
        for (int k = 0; k < j; k++) {
            double angle = M_PI / (1 << (j - k)); // Cálculo del ángulo
            double cos_angle = cos(angle);
            double sin_angle = sin(angle);
            // Aplicar compuerta de fase
            double temp_real = state[k][0] * cos_angle - state[k][1] * sin_angle;
            double temp_imag = state[k][0] * sin_angle + state[k][1] * cos_angle;
            state[k][0] = temp_real;
            state[k][1] = temp_imag;
        }
    }
}
// Función para mostrar el estado final
void display_state(double state[NUM_QUBITS][2]) {
    printf("Estado después de aplicar QFT:\n");
    for (int i = 0; i < (1 << NUM_QUBITS); i++) {
        printf("Estado |%d>: (%.2f, %.2f)\n", i, state[i][0], state[i][1]);
    }
}
int main() {
    double state[NUM_QUBITS][2]; // Matriz para almacenar el estado cuántico
    // Inicializar el estado cuántico
    initialize_state(state);
    // Mostrar estado inicial
    printf("Estado inicial:\n");
    display_state(state);
    // Aplicar la QFT
    apply_qft(state);
    // Mostrar estado final
    display_state(state);
    return 0; // Fin del programa
}