#include <stdio.h>
#include <xmmintrin.h>  // Librería para intrínsecos SSE [16]

void simd_addition(float* a, float* b, float* result, int size) { [16]
int simdSize = size / 4;
    int i;
    // Procesar utilizando SIMD
    for (i = 0; i < simdSize * 4; i += 4) {
        __m128 a_values = _mm_loadu_ps(&a[i]);
        __m128 b_values = _mm_loadu_ps(&b[i]);
        __m128 res_values = _mm_add_ps(a_values, b_values);
        _mm_storeu_ps(&result[i], res_values);
    }
    // Procesar el resto de valores secuencialmente si el tamaño no es múltiplo de 4 [18]
    for (; i < size; ++i) {
        result[i] = a[i] + b[i];
    }
}

int main() { [18]
const int size = 8;
    float a[size] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    float b[size] = {2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    float result[size] = {0};
    // Llamar a la función SIMD
    simd_addition(a, b, result, size); [19]
    // Imprimir los resultados
    for (int i = 0; i < size; ++i) {
        printf("Resultado[%d]: %.1f\n", i, result[i]);
    }
    return 0;
}