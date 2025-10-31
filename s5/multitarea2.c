#include <stdio.h> // Para funciones de entrada/salida como printf
#include <time.h> // Para medir el tiempo de ejecución
#include <stdlib.h> // Para uso general, como exit()

void tarea_larga() {
    for (volatile int i = 0; i < 100000000; i++); // Un bucle largo que simula una tarea de larga duración
    }
void tarea_corta() {
    for (volatile int i = 0; i < 1000000; i++); // Un bucle más corto que simula una tarea de corta duración
    }

double medir_tiempo(void (*func)()) {
    clock_t inicio, fin;
    double tiempo_usado;
    inicio = clock(); // Tiempo al inicio de la ejecución
    func(); // Ejecutar la función
    fin = clock(); // Tiempo al finalizar la ejecución
    tiempo_usado = ((double)(fin - inicio)) / CLOCKS_PER_SEC; // Calcular el tiempo en segundos
     return tiempo_usado; // Retornar el tiempo usado
}

void evaluar_rendimiento(double tiempo, double tiempo_esperado, const char* nombre_tarea) {
    if (tiempo <= tiempo_esperado) {
        printf("%s ha pasado la evaluación. Tiempo: %.2f segundos, Esperado: %.2f segundos\n", nombre_tarea, tiempo, tiempo_esperado);
    } else {
        printf("%s ha fallado la evaluación. Tiempo: %.2f segundos, Esperado: %.2f segundos\n", nombre_tarea, tiempo, tiempo_esperado);
    }
}

int main() {
    // Medir el tiempo de ejecución de cada tarea
    double tiempo_larga = medir_tiempo(tarea_larga);
    double tiempo_corta = medir_tiempo(tarea_corta);
    // Establecer los tiempos esperados (en segundos)
    double tiempo_esperado_larga = 2.0; // Por ejemplo, esperamos que la tarea larga tarde menos de 2 segundos
    double tiempo_esperado_corta = 0.1; // Esperamos que la tarea corta tarde menos de 0.1 segundos
     // Evaluar los tiempos obtenidos frente a los tiempos esperados
    evaluar_rendimiento(tiempo_larga, tiempo_esperado_larga, "Tarea Larga");
    evaluar_rendimiento(tiempo_corta, tiempo_esperado_corta, "Tarea Corta");
    return 0;
}
