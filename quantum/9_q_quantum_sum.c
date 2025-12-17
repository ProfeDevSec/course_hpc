#include <stdio.h>
// Función para realizar la suma de dos bits con acarreo
void add_bits(int a, int b, int* sum, int* carry) {
    *sum = a ^ b; // Suma sin acarreo (XOR)
    *carry = a & b; // Acarreo (AND)
}
// Función para sumar dos números binarios representados como enteros
void add_binary(int num1, int num2) {
    int carry = 0;
    int sum;
    printf("Sumando %d y %d:\n", num1, num2);
    for (int i = 0; i < sizeof(int) * 8; i++) { // Para cada bit
        // Extraer el bit menos significativo de cada número
        int bit1 = (num1 >> i) & 1; // Bit de num1
        int bit2 = (num2 >> i) & 1; // Bit de num2
        // Sumar los bits con acarreo
        add_bits(bit1, bit2, &sum, &carry);
        // Añadir el acarreo de la suma anterior
        int final_sum = sum ^ carry; // Suma final
        carry = sum & carry; // Nuevo acarreo
        // Imprimir el resultado de la suma en cada paso
        printf("Bit %d: Suma = %d, Acarreo = %d\n", i, final_sum, carry);
    }
    printf("Resultado final: %d\n", (num1 + num2));
}
// Función principal
int main() {
    int number1, number2;
    // Solicitar al usuario que ingrese dos números
    printf("Ingrese el primer número: ");
    scanf("%d", &number1);
    printf("Ingrese el segundo número: ");
    scanf("%d", &number2);
    // Realizar la suma binaria
    add_binary(number1, number2);
    return 0;
}