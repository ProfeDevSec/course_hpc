#include <stdio.h> // Para funciones de entrada/salida
#include <pthread.h> // Para el manejo de hilos
#include <time.h> // Para medir el tiempo de ejecución
#include <stdlib.h> // Para funciones generales como malloc

void* tarea_larga(void* arg) {
    printf("Tarea larga ejecutándose...\n");
    for (volatile int i = 0; i < 100000000; i++); // Simulación de una tarea larga
    return NULL;
}
void* tarea_corta(void* arg) {
    printf("Tarea corta ejecutándose...\n");
    for (volatile int i = 0; i < 1000000; i++); // Simulación de una tarea corta
    return NULL;
}

typedef struct {
    double tiempo_ejecucion;
    const char* nombre_tarea;
} ResultadoEvaluacion;

ResultadoEvaluacion evaluar_tarea(void* (*tarea)(void*), const char* nombre_tarea) {
    clock_t inicio, fin;
    ResultadoEvaluacion resultado;

    // Medir el tiempo de ejecución
    inicio = clock();
    tarea(NULL); // Ejecutar la tarea
    fin = clock();

    // Calcular el tiempo de ejecución en segundos
    resultado.tiempo_ejecucion = (double)(fin - inicio) / CLOCKS_PER_SEC;
    resultado.nombre_tarea = nombre_tarea;

    return resultado; // Retornar el resultado de la evaluación
}

int main() {
    pthread_t hilo1, hilo2; // Identificadores de los hilos
    ResultadoEvaluacion resultado1, resultado2;
    // Crear y ejecutar las tareas en hilos separados
    pthread_create(&hilo1, NULL, tarea_larga, NULL);
    pthread_create(&hilo2, NULL, tarea_corta, NULL);
    // Esperar a que las tareas terminen para medir su rendimiento
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);
    // Evaluar el rendimiento de las tareas
    resultado1 = evaluar_tarea(tarea_larga, "Tarea Larga");
    resultado2 = evaluar_tarea(tarea_corta, "Tarea Corta");
    // Imprimir los resultados de la evaluación
    printf("%s: Tiempo de ejecución = %.2f segundos\n", resultado1.nombre_tarea,
   resultado1.tiempo_ejecucion);
    printf("%s: Tiempo de ejecución = %.2f segundos\n", resultado2.nombre_tarea,
   resultado2.tiempo_ejecucion);

    return 0;
}