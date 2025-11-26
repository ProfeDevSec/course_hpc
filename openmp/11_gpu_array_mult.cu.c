/* gpu_array_mult.cu */
#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <time.h>

#define ARRAY_SIZE 10000000
#define SCALAR 2

// Kernel de CUDA
__global__ void multiply_array_gpu(float *array, int size, float scalar) {
    int idx = blockDim.x * blockIdx.x + threadIdx.x;
    if (idx < size) {
        array[idx] *= scalar;
    }
}

int main() {
    float *array_cpu, *array_gpu;
    size_t bytes = ARRAY_SIZE * sizeof(float);

    // Reserva de memoria CPU
    array_cpu = (float*)malloc(bytes);
    
    // Inicialización
    for(int i=0; i<ARRAY_SIZE; i++) array_cpu[i] = 1.0f;

    // Reserva GPU
    cudaMalloc(&array_gpu, bytes);

    // Copia CPU -> GPU
    cudaMemcpy(array_gpu, array_cpu, bytes, cudaMemcpyHostToDevice);

    // Configuración de bloques e hilos
    int blockSize = 1024;
    int gridSize = (ARRAY_SIZE + blockSize - 1) / blockSize;

    clock_t start = clock();
    // Lanzamiento del Kernel
    multiply_array_gpu<<<gridSize, blockSize>>>(array_gpu, ARRAY_SIZE, SCALAR);
    cudaDeviceSynchronize();
    clock_t end = clock();

    // Copia GPU -> CPU
    cudaMemcpy(array_cpu, array_gpu, bytes, cudaMemcpyDeviceToHost);

    double gpu_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Tiempo de procesamiento en GPU: %f segundos\n", gpu_time);

    free(array_cpu);
    cudaFree(array_gpu);
    return 0;
}