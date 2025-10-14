#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <cublas_v2.h>
#define N 1024 // Tamaño de la matriz NxN
// Función para inicializar matrices con valores aleatorios
void initializeMatrix(float *matrix, int size) {
    for (int i = 0; i < size; i++) {
        matrix[i] = (float)(rand() % 100);
    }
}
// Función para imprimir la matriz (opcional para matrices pequeñas)
void printMatrix(float *matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%f ", matrix[i * n + j]);
        }
        printf("\n");
    }
}
int main() {
    // Variables para las matrices
    float *h_A, *h_B, *h_C; // Matrices en el host
    float *d_A, *d_B, *d_C; // Matrices en el dispositivo (GPU)
    int size = N * N * sizeof(float); // Tamaño en bytes de las matrices
    // Crear un handle para CuBLAS
    cublasHandle_t handle;
    cublasCreate(&handle);
    // Asignar memoria en el host
    h_A = (float *)malloc(size);
    h_B = (float *)malloc(size);
    h_C = (float *)malloc(size);
    // Inicializar matrices A y B en el host con valores aleatorios
    initializeMatrix(h_A, N * N);
    initializeMatrix(h_B, N * N);
    // Asignar memoria en el dispositivo (GPU)
    cudaMalloc((void **)&d_A, size);
    cudaMalloc((void **)&d_B, size);
    cudaMalloc((void **)&d_C, size);
    // Copiar matrices A y B desde el host a la GPU
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);
    // Parámetros para CuBLAS: factor de escala y tamaño de la matriz
    const float alpha = 1.0f;
    const float beta = 0.0f;
    // Multiplicación de matrices utilizando CuBLAS
    cublasSgemm(handle,
    CUBLAS_OP_N, CUBLAS_OP_N, // Sin transposición
    N, N, N, // Dimensiones de las matrices
    &alpha, // Escalar alpha
    d_A, N, // Matriz A
    d_B, N, // Matriz B
    &beta, // Escalar beta
    d_C, N); // Matriz C (resultado)
    // Copiar el resultado de la GPU al host
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);
    // Imprimir el resultado (opcional)
    // printMatrix(h_C, N); // Descomenta para matrices pequeñas
    // Liberar recursos
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    free(h_A);
    free(h_B);
    free(h_C);
    // Destruir el handle de CuBLAS
    cublasDestroy(handle);
    printf("Multiplicación de matrices completada con CuBLAS.\n");
    return 0;
}