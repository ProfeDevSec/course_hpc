#include <stdio.h>
// Estructura para representar un qubit
typedef struct {
    int state; // 0 para |0>, 1 para |1>
} Qubit;
// Función para inicializar un qubit en estado |0>
Qubit create_qubit() {
    Qubit q;
    q.state = 0; // Estado inicial
    return q;
}
// Función para aplicar la compuerta CNOT
void apply_cnot(Qubit* control, Qubit* target) {
    if (control->state == 1) {
        target->state = !target->state; // Invertir el estado del qubit objetivo
    }
}
// Función para aplicar la compuerta Toffoli
void apply_toffoli(Qubit* control1, Qubit* control2, Qubit* target) {
    if (control1->state == 1 && control2->state == 1) {
        target->state = !target->state; // Invertir el estado del qubit objetivo
    }
}
// Función para imprimir el estado de los qubits
void print_qubit_state(Qubit q) {
    printf("Estado del qubit: |%d>\n", q.state);
}
int main() {
    // Crear qubits
    Qubit q1 = create_qubit(); // Control 1
    Qubit q2 = create_qubit(); // Control 2
    Qubit q3 = create_qubit(); // Objetivo
    // Inicializar los qubits
    q1.state = 1; // Control 1 en |1>
    q2.state = 1; // Control 2 en |1>
    q3.state = 0; // Objetivo en |0>
    printf("Antes de aplicar las compuertas:\n");
    print_qubit_state(q1);
    print_qubit_state(q2);
    print_qubit_state(q3);
    // Aplicar la compuerta CNOT
    apply_cnot(&q1, &q3);
    printf("\nDespués de aplicar CNOT (Control 1 a Objetivo):\n");
    print_qubit_state(q1);
    print_qubit_state(q3);
    // Aplicar la compuerta Toffoli
    apply_toffoli(&q1, &q2, &q3);
    printf("\nDespués de aplicar Toffoli (Control 1 y Control 2 a Objetivo):\n");
    print_qubit_state(q1);
    print_qubit_state(q2);
    print_qubit_state(q3);
    return 0;
}