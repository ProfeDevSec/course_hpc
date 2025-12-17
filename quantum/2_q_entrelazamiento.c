#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// Estructura para representar un qubit
typedef struct {
    int state; // 0 o 1
} Qubit;
// Función para crear un qubit en un estado aleatorio
Qubit createRandomQubit() {
    Qubit qubit;
    qubit.state = rand() % 2; // Estado aleatorio (0 o 1)
    return qubit;
}
// Función para entrelazar dos qubits
void entangle(Qubit *q1, Qubit *q2) {
    // Si q1 está en estado 0, q2 también estará en 0, y viceversa
    q2->state = q1->state;
}
// Función para mostrar el estado de un qubit
void printQubit(Qubit q, int index) {
    printf("Qubit %d estado: %d\n", index, q.state);
}
int main() {
    srand(time(NULL)); // Inicializar la semilla aleatoria
    // Crear dos qubits
    Qubit qubit1 = createRandomQubit();
    Qubit qubit2;
    // Entrelazar los qubits
    entangle(&qubit1, &qubit2);
    // Mostrar los estados de los qubits
    printQubit(qubit1, 1);
    printQubit(qubit2, 2);
    // Cambiar el estado de qubit1 para demostrar la correlación
    qubit1.state = (qubit1.state == 0) ? 1 : 0; // Cambiar el estado (0 a 1 o 1 a 0)
    // Mostrar los estados de los qubits después de cambiar qubit1
    printf("\nDespués de cambiar el estado de Qubit 1:\n");
    printQubit(qubit1, 1);
    printQubit(qubit2, 2); // Qubit 2 debería seguir el estado de Qubit 1
    return 0;
}