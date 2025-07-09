#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_FILAS 100
#define MAX_COLUMNAS 100

int matriz[MAX_FILAS][MAX_COLUMNAS];
int filas;
int columnas;

void *numeros_impares(void *param);
void *numeros_pares(void *param);
void *calcular_suma(void *param);
void *calcular_promedio(void *param);

int main(void) {
    int continuar = 1;

    while (continuar) {
        printf("Introduce el número de filas de la matriz (máximo %d): ", MAX_FILAS);
        scanf("%d", &filas);
        printf("Introduce el número de columnas de la matriz (máximo %d): ", MAX_COLUMNAS);
        scanf("%d", &columnas);

        // Verificar que las dimensiones sean válidas
        if (filas > MAX_FILAS || columnas > MAX_COLUMNAS) {
            fprintf(stderr, "Dimensiones de la matriz exceden el máximo permitido (%d x %d)\n", MAX_FILAS, MAX_COLUMNAS);
            continue;
        }

        // Introducir los valores de la matriz
        printf("Introduce los valores de la matriz:\n");
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++) {
                printf("Elemento [%d][%d]: ", i + 1, j + 1);
                scanf("%d", &matriz[i][j]);
            }
        }

        pthread_t hilos[4];

        // Crear hilos
        pthread_create(&hilos[0], NULL, numeros_impares, NULL);
        pthread_create(&hilos[1], NULL, numeros_pares, NULL);
        pthread_create(&hilos[2], NULL, calcular_suma, NULL);
        pthread_create(&hilos[3], NULL, calcular_promedio, NULL);

        // Esperar a que todos los hilos terminen
        for (int i = 0; i < 4; i++) {
            pthread_join(hilos[i], NULL);
        }

        printf("¿Deseas continuar? (1 para sí, 0 para no): ");
        scanf("%d", &continuar);
    }

    return 0;
}

void *numeros_impares(void *param) {
    printf("Números impares: ");
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (matriz[i][j] % 2 != 0) {
                printf("%d ", matriz[i][j]);
            }
        }
    }
    printf("\n");
    pthread_exit(0);
}

void *numeros_pares(void *param) {
    printf("Números pares: ");
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (matriz[i][j] % 2 == 0) {
                printf("%d ", matriz[i][j]);
            }
        }
    }
    printf("\n");
    pthread_exit(0);
}

void *calcular_suma(void *param) {
    int suma = 0;
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            suma += matriz[i][j];
        }
    }
    printf("La suma total de la matriz es: %d\n", suma);
    pthread_exit(0);
}

void *calcular_promedio(void *param) {
    int suma = 0;
    int numElementos = filas * columnas;
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            suma += matriz[i][j];
        }
    }
    double promedio = (double)suma / numElementos;
    printf("El promedio es: %.2f\n", promedio);
    pthread_exit(0);
}
