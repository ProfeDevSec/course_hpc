/************************************************************************************
 * SGEMM con cuBLAS (A * B = C) usando datos en ROW-MAJOR (C/C++) con "truco" de transposición
 *
 * Propósito
 * ---------
 * - Mostrar una implementación mínima, segura y rápida de multiplicación de matrices
 *   de punto flotante simple (SGEMM) en GPU usando cuBLAS.
 * - El código de aplicación trabaja en orden de memoria ROW-MAJOR (estilo C/C++).
 *   cuBLAS, por defecto, interpreta los punteros en COLUMN-MAJOR (Fortran).
 *   Para mantener nuestros datos en row-major sin reordenarlos, usamos el "truco":
 *      C_row = A_row * B_row   equivale a   C_col = (B_col)^T * (A_col)^T
 *   Es decir, pedimos a cuBLAS que multiplique B^T por A^T (ambas transpuestas),
 *   lo que produce el mismo resultado que A*B en convención row-major.
 *
 * Pasos principales
 * -----------------
 * 1) Reservar y llenar matrices en host (A, B) [row-major].
 * 2) Reservar matrices en device (d_A, d_B, d_C).
 * 3) Copiar A y B a la GPU.
 * 4) Crear handle cuBLAS y (opcional) configurar math mode.
 * 5) Llamar a cublasSgemm con (op_T, op_T) y orden B primero, luego A.
 * 6) Copiar C de vuelta a host.
 * 7) Liberar recursos (device/host) y destruir handle.
 *
 * Notas de rendimiento
 * --------------------
 * - Usamos memoria "page-locked" (cudaMallocHost) para acelerar cudaMemcpy.
 * - Compilar con -O3 y enlazar con -lcublas.
 * - Para FP32 en GPUs Ampere/Hopper puedes activar Tensor Cores (TF32):
 *     cublasSetMathMode(handle, CUBLAS_TF32_TENSOR_OP_MATH);
 *   (menor precisión, mayor rendimiento).
 *
 * Compilación (Windows/Linux)
 * ---------------------------
 *   nvcc -O3 -o sgemm main.cu -lcublas
 *
 * Validación
 * ----------
 * - Si necesitas validar, compara contra una implementación CPU para tamaños pequeños
 *   (p.ej., N=256), o usa una segunda llamada cuBLAS con layouts alternativos.
 ************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <cublas_v2.h>

#ifndef N
#define N 1024   // Tamaño por defecto: matrices cuadradas N x N
#endif

/********** Macros de comprobación de errores (CUDA y cuBLAS) ******************/
#define CUDA_CHECK(x) do { \
    cudaError_t e = (x); \
    if (e != cudaSuccess) { \
        fprintf(stderr, "CUDA %s:%d: %s\n", __FILE__, __LINE__, cudaGetErrorString(e)); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

#define CUBLAS_CHECK(x) do { \
    cublasStatus_t s = (x); \
    if (s != CUBLAS_STATUS_SUCCESS) { \
        fprintf(stderr, "cuBLAS %s:%d: status=%d\n", __FILE__, __LINE__, (int)s); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

/**
 * initializeMatrix:
 * Llena un buffer row-major de 'elements' floats con valores pseudoaleatorios [0..99].
 * Usamos srand(1234) en main() para reproducibilidad.
 */
static void initializeMatrix(float *m, size_t elements) {
    for (size_t i = 0; i < elements; ++i) m[i] = (float)(rand() % 100);
}

int main() {
    /***** Semilla fija para que las ejecuciones sean reproducibles *****/
    srand(1234);

    /***** Parámetros y tamaños *****/
    const int n = N;                                 // Dimensión lógica
    const size_t elems = (size_t)n * (size_t)n;      // Número de elementos por matriz
    const size_t bytes = elems * sizeof(float);      // Tamaño en bytes por matriz

    /***** Reservas en host (page-locked para copias más rápidas) ********
     * Nota: Si prefieres no usar page-locked, sustituye cudaMallocHost por malloc
     * y CUDA_CHECK(cudaFreeHost(...)) por free(...).
     *********************************************************************/
    float *h_A = nullptr, *h_B = nullptr, *h_C = nullptr;
    CUDA_CHECK(cudaMallocHost(&h_A, bytes));
    CUDA_CHECK(cudaMallocHost(&h_B, bytes));
    CUDA_CHECK(cudaMallocHost(&h_C, bytes));

    /***** Inicialización de A y B en el host (row-major) ****************/
    initializeMatrix(h_A, elems);
    initializeMatrix(h_B, elems);

    /***** Reservas en device ********************************************/
    float *d_A = nullptr, *d_B = nullptr, *d_C = nullptr;
    CUDA_CHECK(cudaMalloc(&d_A, bytes));
    CUDA_CHECK(cudaMalloc(&d_B, bytes));
    CUDA_CHECK(cudaMalloc(&d_C, bytes));

    /***** Copias Host -> Device *****************************************/
    CUDA_CHECK(cudaMemcpy(d_A, h_A, bytes, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_B, h_B, bytes, cudaMemcpyHostToDevice));

    /***** Crear handle cuBLAS *******************************************/
    cublasHandle_t handle;
    CUBLAS_CHECK(cublasCreate(&handle));

    // (Opcional) Activa Tensor Cores en FP32 (TF32). Úsalo si aceptas menor precisión:
    // CUBLAS_CHECK(cublasSetMathMode(handle, CUBLAS_TF32_TENSOR_OP_MATH));

    /***** Parámetros escalares SGEMM: C = alpha * (opA(A) * opB(B)) + beta * C *****/
    const float alpha = 1.0f;
    const float beta  = 0.0f;

    /***** SGEMM con "truco" para conservar ROW-MAJOR en la app **********
     *
     * Queremos: C_row = A_row * B_row  (datos en row-major)
     * cuBLAS asume COLUMN-MAJOR. La identidad útil es:
     *   C_row = A_row * B_row   <=>   C_col = (B_col)^T * (A_col)^T
     * Esto se implementa llamando a SGEMM con:
     *   op(A) = Transpuesta,  op(B) = Transpuesta,  y pasando primero B y luego A.
     *
     * Firma de cublasSgemm (col-major):
     *   cublasSgemm(handle, opA, opB,
     *               m, n, k,
     *               &alpha,
     *               A, lda,
     *               B, ldb,
     *               &beta,
     *               C, ldc);
     *
     * Para matrices cuadradas N:
     *   m = n = k = N
     *   lda = ldb = ldc = N (leading dimension)
     *********************************************************************/
    CUBLAS_CHECK(cublasSgemm(
        handle,
        CUBLAS_OP_T, CUBLAS_OP_T,   // transponer ambos operandos
        n, n, n,                    // m, n, k
        &alpha,
        d_B, n,                     // B primero (será B^T internamente)
        d_A, n,                     // luego A (será A^T internamente)
        &beta,
        d_C, n                      // salida C (col-major visto por cuBLAS; row-major para nosotros)
    ));

    /***** Copia Device -> Host ******************************************/
    CUDA_CHECK(cudaMemcpy(h_C, d_C, bytes, cudaMemcpyDeviceToHost));

    /***** Limpieza de recursos ******************************************/
    CUBLAS_CHECK(cublasDestroy(handle));
    CUDA_CHECK(cudaFree(d_A));
    CUDA_CHECK(cudaFree(d_B));
    CUDA_CHECK(cudaFree(d_C));
    CUDA_CHECK(cudaFreeHost(h_A));
    CUDA_CHECK(cudaFreeHost(h_B));
    CUDA_CHECK(cudaFreeHost(h_C));

    /***** Fin ***********************************************************/
    printf("SGEMM con cuBLAS completado (row-major vía transposición).\n");
    return 0;
}
