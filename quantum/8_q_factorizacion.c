#include <stdio.h>
#include <math.h>

// Función para encontrar factores de un número
void find_factors(int n) {
    printf("Factores de %d son: ", n);
    for (int i = 1; i <= sqrt(n); i++) {
        if (n % i == 0) {
            // i es un factor
            printf("%d ", i);
            if (i != n / i) {
                // n/i es otro factor
                printf("%d ", n / i);
            }
        }
    }
    printf("\n");
}
// Función principal
int main() {
    int number;
    // Solicitar al usuario un número
    printf("Ingrese un número para factorizar: ");
    scanf("%d", &number);
    // Encontrar y mostrar factores
    find_factors(number);
    return 0;
}