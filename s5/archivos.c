#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 10 // Máximo número de archivos permitidos
#define MAX_FILE_SIZE 1024 // Máximo tamaño de archivo en bytes

// Estructura que representa un archivo
struct File {
    char name[100]; // Nombre del archivo
    char data[MAX_FILE_SIZE]; // Contenido del archivo
    int size; // Tamaño del archivo
};
// Estructura que representa el sistema de archivos
struct FileSystem {
    struct File files[MAX_FILES]; // Arreglo de archivos
    int file_count; // Cantidad actual de archivos en el sistema
};

// Inicializa el sistema de archivos
void initializeFileSystem(struct FileSystem* fs) {
    fs->file_count = 0; // El sistema comienza sin archivos
}

// Crea un archivo en el sistema
void createFile(struct FileSystem* fs, char* name) {
    if (fs->file_count >= MAX_FILES) { // Verifica si se alcanzó el límite de archivos
        printf("Error: No se pueden crear más archivos, el sistema está lleno.\n");
        return;
    }

    // Verifica si ya existe un archivo con el mismo nombre
    for (int i = 0; i < fs->file_count; i++) {
        if (strcmp(fs->files[i].name, name) == 0) {
            printf("Error: Ya existe un archivo con el mismo nombre.\n");
            return;
        }
    }
    // Crea el nuevo archivo
    strcpy(fs->files[fs->file_count].name, name);
    fs->files[fs->file_count].size = 0; // Tamaño inicial del archivo es 0
    printf("Archivo '%s' creado con éxito.\n", name);
    fs->file_count++; // Incrementa el número de archivos en el sistema
}

// Escribe datos en un archivo específico
void writeFile(struct FileSystem* fs, char* name, char* data) {
    for (int i = 0; i < fs->file_count; i++) {
        if (strcmp(fs->files[i].name, name) == 0) {
            int dataSize = strlen(data);
            if (dataSize > MAX_FILE_SIZE) { // Verifica si el tamaño de los datos es mayor que el permitido
                 printf("Error: El tamaño de los datos excede el tamaño máximo del archivo.\n");
                 return;
                 }
            // Escribe los datos en el archivo
            strcpy(fs->files[i].data, data);
            fs->files[i].size = dataSize; // Actualiza el tamaño del archivo
            printf("Datos escritos con éxito en el archivo '%s'.\n", name);
            return;
        }
    }
    printf("Error: El archivo '%s' no existe.\n", name);
}

// Lee datos de un archivo específico
void readFile(struct FileSystem* fs, char* name) {
    for (int i = 0; i < fs->file_count; i++) {
        if (strcmp(fs->files[i].name, name) == 0) {
            printf("Contenido del archivo '%s':\n%s\n", name, fs->files[i].data);
            return;
        }
    }
    printf("Error: El archivo '%s' no existe.\n", name);
}

int main() {
    struct FileSystem fs;
    initializeFileSystem(&fs); // Inicializa el sistema
    // Creación de archivos
    createFile(&fs, "archivo1.txt");
    createFile(&fs, "archivo2.txt");
    // Escritura de datos en archivos
    writeFile(&fs, "archivo1.txt", "Este es el contenido del archivo 1.");
    writeFile(&fs, "archivo2.txt", "Contenido para archivo 2.");
    // Lectura de archivos
    readFile(&fs, "archivo1.txt");
    readFile(&fs, "archivo2.txt");
    return 0;
}
