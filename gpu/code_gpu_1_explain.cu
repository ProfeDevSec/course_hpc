/************************************************************************************
 * Multiplicación de matrices cuadradas NxN en CUDA (versión didáctica)
 *
 * Objetivo:
 *  - Mostrar el flujo completo: reservar memoria en host y device, copiar datos,
 *    configurar rejilla/bloques, lanzar kernel, sincronizar, copiar resultados y validar.
 *  - Incluir medición de tiempo GPU (cudaEvent) y comparación con una versión CPU.
 *
 * Requisitos:
 *  - Compilar con nvcc (CUDA Toolkit). En Windows, asegúrate de tener MSVC instalado
 *    y compila desde "x64 Native Tools Command Prompt for VS".
 *
 * Compilación:
 *  nvcc -O2 -o matrixMul main.cu
 *
 * Ejecución:
 *  ./matrixMul
 ************************************************************************************/

#include <stdio.h>                  // printf, fprintf
#include <stdlib.h>                 // malloc, free, rand, srand
#include <math.h>                   // fabsf
#include <cuda_runtime.h>           // API de CUDA Runtime
#include <device_launch_parameters.h> // Parámetros de lanzamiento (MSVC/Windows)

#ifndef N
#define N 1024   // Tamaño de matriz: N x N (puedes cambiarlo; debe ser múltiplo o no de 16, da igual)
#endif

/*********** Utilidad: macro para chequear errores de llamadas CUDA ***********/
#define CUDA_CHECK(call) do {                                                     \
    cudaError_t _e = (call);                                                      \
    if (_e != cudaSuccess) {                                                      \
        fprintf(stderr, "CUDA error en %s:%d: %s\n", __FILE__, __LINE__,          \
                cudaGetErrorString(_e));                                          \
        exit(EXIT_FAILURE);                                                       \
    }                                                                             \
} while (0)

/*********** Kernel de CUDA: C = A * B (matrices cuadradas NxN) ****************
 * Cada hilo calcula un elemento C[row, col].
 * Índices 2D:
 *   - blockIdx.x, blockIdx.y: coordenadas del bloque
 *   - threadIdx.x, threadIdx.y: coordenadas del hilo dentro del bloque
 *   - blockDim.x, blockDim.y: tamaño del bloque (hilos por dimensión)
 *******************************************************************************/
__global__ void matrixMultiplyGPU(const float *A, const float *B, float *C, int n) {
    // Calcular la fila y columna globales que le tocan a este hilo
    int row = blockIdx.y * blockDim.y + threadIdx.y;  // eje Y -> filas
    int col = blockIdx.x * blockDim.x + threadIdx.x;  // eje X -> columnas

    // Validación de límites (la rejilla puede cubrir "de más")
    if (row >= n || col >= n) return;

    // Acumulador para el producto punto de la fila 'row' de A y la columna 'col' de B
    float sum = 0.0f;

    // Bucle interno: recorrer k para A[row, k] * B[k, col]
    for (int k = 0; k < n; ++k) {
        sum += A[row * n + k] * B[k * n + col];
    }

    // Escribir el resultado en C
    C[row * n + col] = sum;
}

/*********** Inicialización de matrices en host ********************************
 * Rellena con enteros aleatorios [0..99] convertidos a float.
 *******************************************************************************/
void initializeMatrix(float *m, int elements) {
    for (int i = 0; i < elements; ++i) {
        m[i] = (float)(rand() % 100);
    }
}

/*********** Versión CPU de referencia *****************************************
 * Implementación sencilla O(N^3) para verificar la corrección del resultado GPU.
 *******************************************************************************/
void matrixMultiplyCPU(const float *A, const float *B, float *C, int n) {
    for (int i = 0; i < n; ++i) {          // filas de A
        for (int j = 0; j < n; ++j) {      // columnas de B
            float sum = 0.0f;
            for (int k = 0; k < n; ++k) {  // producto punto
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
}

/*********** Comparación numérica **********************************************
 * Calcula el error máximo absoluto entre C_gpu y C_cpu.
 * Devuelve true si el error es aceptable (<= tol).
 *******************************************************************************/
bool compareMatrices(const float *A, const float *B, int elements, float tol,
                     float *out_max_abs_err) {
    float max_err = 0.0f;
    for (int i = 0; i < elements; ++i) {
        float err = fabsf(A[i] - B[i]);
        if (err > max_err) max_err = err;
        if (err > tol) { // Early exit posible si quieres
            // sigue para medir el máximo real
        }
    }
    if (out_max_abs_err) *out_max_abs_err = max_err;
    return max_err <= tol;
}

/*********** (Opcional) Imprimir submatriz pequeña *****************************/
void printMatrixBlock(const float *M, int n, int rows=8, int cols=8) {
    int rmax = (rows < n) ? rows : n;
    int cmax = (cols < n) ? cols : n;
    for (int i = 0; i < rmax; ++i) {
        for (int j = 0; j < cmax; ++j) {
            printf("%7.2f ", M[i * n + j]);
        }
        printf("\n");
    }
}

int main() {
    /***** 1) Preparación de datos en Host *****/
    srand(1234); // Semilla fija para reproducibilidad

    const int n = N;                                        // tamaño lógico
    const size_t elements = (size_t)n * (size_t)n;          // número de floats por matriz
    const size_t bytes    = elements * sizeof(float);       // tamaño en bytes por matriz

    // Reservar matrices en Host (CPU)
    float *h_A  = (float*)malloc(bytes);
    float *h_B  = (float*)malloc(bytes);
    float *h_Cg = (float*)malloc(bytes); // resultado desde GPU
    float *h_Cc = (float*)malloc(bytes); // resultado CPU (para validar)
    if (!h_A || !h_B || !h_Cg || !h_Cc) {
        fprintf(stderr, "Error: no se pudo reservar memoria en host.\n");
        return EXIT_FAILURE;
    }

    // Inicializar A y B con valores aleatorios
    initializeMatrix(h_A, (int)elements);
    initializeMatrix(h_B, (int)elements);

    /***** 2) Reservar memoria en Device (GPU) *****/
    float *d_A = nullptr, *d_B = nullptr, *d_C = nullptr;
    CUDA_CHECK(cudaMalloc((void**)&d_A, bytes));
    CUDA_CHECK(cudaMalloc((void**)&d_B, bytes));
    CUDA_CHECK(cudaMalloc((void**)&d_C, bytes));

    /***** 3) Copiar A y B del Host al Device *****/
    CUDA_CHECK(cudaMemcpy(d_A, h_A, bytes, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_B, h_B, bytes, cudaMemcpyHostToDevice));

    /***** 4) Configuración de ejecución ****************************************
     * Elegimos bloques 16x16 (=256 hilos por bloque), bastante estándar.
     * Calculamos cuántos bloques se necesitan para cubrir N en cada dimensión:
     *   grid.x = ceil(N / blockDim.x)
     *   grid.y = ceil(N / blockDim.y)
     ****************************************************************************/
    dim3 block(16, 16);
    dim3 grid((n + block.x - 1) / block.x,
              (n + block.y - 1) / block.y);

    /***** 5) Medición de tiempo GPU con cudaEvent_t ****************************/
    cudaEvent_t start, stop;
    CUDA_CHECK(cudaEventCreate(&start));
    CUDA_CHECK(cudaEventCreate(&stop));

    // Registrar tiempo antes del kernel
    CUDA_CHECK(cudaEventRecord(start));

    /***** 6) Lanzamiento del kernel ********************************************/
    matrixMultiplyGPU<<<grid, block>>>(d_A, d_B, d_C, n);

    // Comprobar si el lanzamiento generó algún error asíncrono
    CUDA_CHECK(cudaGetLastError());

    // Sincronizar para que el kernel termine antes de medir tiempo y continuar
    CUDA_CHECK(cudaDeviceSynchronize());

    // Registrar tiempo después del kernel y calcular milisegundos
    CUDA_CHECK(cudaEventRecord(stop));
    CUDA_CHECK(cudaEventSynchronize(stop));
    float msGPU = 0.0f;
    CUDA_CHECK(cudaEventElapsedTime(&msGPU, start, stop));

    // Destruir eventos
    CUDA_CHECK(cudaEventDestroy(start));
    CUDA_CHECK(cudaEventDestroy(stop));

    /***** 7) Copiar resultado de vuelta al Host *****/
    CUDA_CHECK(cudaMemcpy(h_Cg, d_C, bytes, cudaMemcpyDeviceToHost));

    /***** 8) (Opcional) Calcular resultado en CPU para validar *****************/
    // Ojo: CPU O(N^3) puede tardar para N grandes. Con N=1024 es costoso.
    // Si quieres validar rápido, prueba con N=256 o N=512.
    printf("Calculando referencia CPU (puede tardar con N=%d)...\n", n);
    float msCPU = 0.0f;
    {
        // Cronómetro simple con eventos de CPU (poco preciso). Si deseas
        // medir mejor en Windows, usa QueryPerformanceCounter.
        clock_t c0 = clock();
        matrixMultiplyCPU(h_A, h_B, h_Cc, n);
        clock_t c1 = clock();
        msCPU = 1000.0f * (float)(c1 - c0) / (float)CLOCKS_PER_SEC;
    }

    /***** 9) Comparar resultados ***********************************************/
    float max_abs_err = 0.0f;
    const float tol = 1e-3f; // tolerancia típica para floats con sumas acumuladas
    bool ok = compareMatrices(h_Cg, h_Cc, (int)elements, tol, &max_abs_err);

    /***** 10) Reporte **********************************************************/
    printf("\n=== Reporte ===\n");
    printf("N = %d\n", n);
    printf("Tiempo GPU (kernel) : %.3f ms\n", msGPU);
    printf("Tiempo CPU (ref)    : %.3f ms\n", msCPU);
    printf("Error max abs       : %.6f\n", max_abs_err);
    printf("Validación          : %s\n", ok ? "OK (dentro de tolerancia)" : "FALLO (excede tolerancia)");

    // (Opcional) imprime un bloque 8x8 de la salida para inspección visual
    // printf("\nBloque 8x8 de C (GPU):\n");
    // printMatrixBlock(h_Cg, n, 8, 8);

    /***** 11) Liberar recursos *************************************************/
    CUDA_CHECK(cudaFree(d_A));
    CUDA_CHECK(cudaFree(d_B));
    CUDA_CHECK(cudaFree(d_C));
    free(h_A);
    free(h_B);
    free(h_Cg);
    free(h_Cc);

    return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
