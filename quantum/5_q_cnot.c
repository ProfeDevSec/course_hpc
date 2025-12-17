#include <stdio.h>
#include <stdlib.h>
// Estructura para representar un qubit
typedef struct {
    int state; // 0 o 1
} Qubit;
// Función para crear un qubit en un estado dado
Qubit createQubit(int state) {
    Qubit qubit;
    qubit.state = state; // Estado inicial (0 o 1)
    return qubit;
}
// Función para aplicar la compuerta CNOT
void applyCNOT(Qubit control, Qubit *target) {
    // Cambiar el estado del qubit objetivo si el qubit de control está en |1>
    if (control.state == 1) {
        target->state = (target->state == 0) ? 1 : 0; // Invertir el estado del qubit objetivo
        }
}
// Función para mostrar el estado de un qubit
void printQubit(Qubit q, int index) {
    printf("Qubit %d estado: %d\n", index, q.state);
}
int main() {
    // Crear dos qubits
    Qubit control = createQubit(1); // Qubit de control en estado |1>
    Qubit target = createQubit(0); // Qubit objetivo en estado |0>
    // Mostrar el estado inicial de los qubits
    printf("Estado inicial:\n");
    printQubit(control, 1);
    printQubit(target, 2);
    // Aplicar la compuerta CNOT
    applyCNOT(control, &target);
    // Mostrar el estado de los qubits después de aplicar CNOT
    printf("\nDespués de aplicar la compuerta CNOT:\n");
    printQubit(control, 1);
    printQubit(target, 2);
    // Cambiar el estado del qubit de control y aplicar CNOT nuevamente
    control.state = 0; // Cambiar control a estado |0|
    applyCNOT(control, &target);
    // Mostrar el estado de los qubits después de aplicar CNOT nuevamente
    printf("\nDespués de cambiar el estado del qubit de control y aplicar CNOT:\n");
    printQubit(control, 1);
    printQubit(target, 2);
    return 0;
}