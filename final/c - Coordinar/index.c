// el hilo/proceso coordinador gestiona a los otros dos hilos que se
// encargan de llenar valores la matriz de enteros pidiendo su valor al usuario

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#define ROWS 4
#define COLS 3

int matriz[ROWS][COLS];
int valor_actual, fila_actual, col_actual;

pthread_mutex_t lock_coordinador, lock_positivo, lock_negativo;

void *coordinador(void *arg);
void *positivo(void *arg);
void *negativo(void *arg);

int main(void)
{
    // Bloquear los hilos positivo y negativo para que esperen
    pthread_mutex_lock(&lock_positivo);
    pthread_mutex_lock(&lock_negativo);

    pthread_t hilo_coordinador, hilo_positivo, hilo_negativo;

    pthread_create(&hilo_coordinador, NULL, coordinador, NULL);
    pthread_create(&hilo_positivo, NULL, positivo, NULL);
    pthread_create(&hilo_negativo, NULL, negativo, NULL);

    pthread_join(hilo_coordinador, NULL);

    // Cuando coordinador termina, desbloqueamos para que terminen
    pthread_mutex_unlock(&lock_positivo);
    pthread_mutex_unlock(&lock_negativo);

    pthread_join(hilo_positivo, NULL);
    pthread_join(hilo_negativo, NULL);

    // Mostrar matriz
    printf("\nMatriz final:\n");
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
    }

    pthread_mutex_destroy(&lock_coordinador);
    pthread_mutex_destroy(&lock_positivo);
    pthread_mutex_destroy(&lock_negativo);

    return 0;
}

void *coordinador(void *arg)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("Ingresa un número para [%d][%d]: ", i, j);
            scanf("%d", &valor_actual);

            fila_actual = i;
            col_actual = j;

            if (valor_actual >= 0)
            {
                pthread_mutex_unlock(&lock_positivo);  // Avisar a positivo
                pthread_mutex_lock(&lock_coordinador); // Esperar a que termine positivo
            }
            else
            {
                pthread_mutex_unlock(&lock_negativo);  // Avisar a negativo
                pthread_mutex_lock(&lock_coordinador); // Esperar a que termine negativo
            }
        }
    }

    fila_actual = ROWS;

    return NULL;
}

void *positivo(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&lock_positivo);

        if (fila_actual >= ROWS)
            break;

        matriz[fila_actual][col_actual] = valor_actual;
        printf("[POSITIVO] Guarda %d en [%d][%d]\n", valor_actual, fila_actual, col_actual);

        pthread_mutex_unlock(&lock_coordinador);
    }

    return NULL;
}

void *negativo(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&lock_negativo);

        if (fila_actual >= ROWS)
            break;

        matriz[fila_actual][col_actual] = valor_actual;
        printf("[NEGATIVO] Guarda %d en [%d][%d]\n", valor_actual, fila_actual, col_actual);

        pthread_mutex_unlock(&lock_coordinador);
    }

    return NULL;
}