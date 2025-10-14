#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#define N 1024  // Matrices NxN

#define CUDA_CHECK(call) do {                                      \
    cudaError_t _e = (call);                                       \
    if (_e != cudaSuccess) {                                       \
        fprintf(stderr, "CUDA error %s:%d: %s\n",                  \
                __FILE__, __LINE__, cudaGetErrorString(_e));       \
        exit(EXIT_FAILURE);                                        \
    }                                                              \
} while (0)

__global__ void matrixMultiplyGPU(const float *A, const float *B, float *C, int n) {
    int row = blockIdx.y * blockDim.y + threadIdx.y; // y -> filas
    int col = blockIdx.x * blockDim.x + threadIdx.x; // x -> columnas
    if (row >= n || col >= n) return;

    float sum = 0.0f;
    for (int i = 0; i < n; i++) {
        sum += A[row * n + i] * B[i * n + col];
    }
    C[row * n + col] = sum;
}

void initializeMatrix(float *matrix, int elements) {
    for (int i = 0; i < elements; i++) {
        matrix[i] = (float)(rand() % 100);
    }
}

void printMatrix(const float *matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%.2f ", matrix[i * n + j]);
        }
        printf("\n");
    }
}

int main() {
    srand(1234);

    const size_t bytes = (size_t)N * (size_t)N * sizeof(float);

    // Host
    float *h_A = (float*)malloc(bytes);
    float *h_B = (float*)malloc(bytes);
    float *h_C = (float*)malloc(bytes);
    if (!h_A || !h_B || !h_C) {
        fprintf(stderr, "No se pudo asignar memoria en host.\n");
        return EXIT_FAILURE;
    }

    initializeMatrix(h_A, N * N);
    initializeMatrix(h_B, N * N);

    // Device
    float *d_A = nullptr, *d_B = nullptr, *d_C = nullptr;
    CUDA_CHECK(cudaMalloc((void**)&d_A, bytes));
    CUDA_CHECK(cudaMalloc((void**)&d_B, bytes));
    CUDA_CHECK(cudaMalloc((void**)&d_C, bytes));

    CUDA_CHECK(cudaMemcpy(d_A, h_A, bytes, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_B, h_B, bytes, cudaMemcpyHostToDevice));

    // Configuración de ejecución
    dim3 threadsPerBlock(16, 16);
    dim3 blocksPerGrid((N + threadsPerBlock.x - 1) / threadsPerBlock.x,
                       (N + threadsPerBlock.y - 1) / threadsPerBlock.y);

    // ¡Lanzamiento correcto del kernel!
    matrixMultiplyGPU<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, N);
    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize());

    // Copiar resultado de vuelta
    CUDA_CHECK(cudaMemcpy(h_C, d_C, bytes, cudaMemcpyDeviceToHost));

    // (Opcional) imprimir un subbloque pequeño para verificar
    printMatrix(h_C, 8);

    // Liberar
    CUDA_CHECK(cudaFree(d_A));
    CUDA_CHECK(cudaFree(d_B));
    CUDA_CHECK(cudaFree(d_C));
    free(h_A);
    free(h_B);
    free(h_C);

    printf("Multiplicación completada correctamente.\n");
    return 0;
}
