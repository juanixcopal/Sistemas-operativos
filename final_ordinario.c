#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#define ROWS 4
#define COLS 3

int matriz[ROWS][COLS];
int fila_actual, col_actual;

pthread_mutex_t lock_coordinador, lock_positivo, lock_negativo;

pthread_t hilo_coordinador, hilo_positivo, hilo_negativo;

void *coordinador(void *arg);
void *positivo(void *arg);
void *negativo(void *arg);

int main(void)
{
    // Bloquear los hilos positivo y negativo para que esperen
    pthread_mutex_lock(&lock_positivo);
    pthread_mutex_lock(&lock_negativo);
    pthread_mutex_lock(&lock_coordinador);

    pthread_create(&hilo_coordinador, NULL, coordinador, NULL);
    pthread_create(&hilo_positivo, NULL, positivo, NULL);
    pthread_create(&hilo_negativo, NULL, negativo, NULL);

    pthread_join(hilo_coordinador, NULL);

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

    pthread_join(hilo_positivo, NULL);
    pthread_join(hilo_negativo, NULL);

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
            fila_actual = i;
            col_actual = j;

            int producto = i * j;

            if (producto % 2 == 0) // Par o 0 -> positivo
            {
                pthread_mutex_unlock(&lock_positivo);  // Avisar a positivo
                pthread_mutex_lock(&lock_coordinador); // Esperar a que termine positivo
            }
            else // Impar -> negativo
            {
                pthread_mutex_unlock(&lock_negativo);  // Avisar a negativo
                pthread_mutex_lock(&lock_coordinador); // Esperar a que termine negativo
            }
        }
    }

    fila_actual = -1; // Marca de finalización

    return NULL;
}

void *positivo(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&lock_positivo);

        if (fila_actual < 0)
        {
            // Aquí en vez de salir, se bloquea indefinidamente para seguir "vivo"
            while (1)
            {
                sleep(10);
            }
        }

        int valor;
        printf("[POSITIVO] Ingresa un número para [%d][%d]: ", fila_actual, col_actual);
        scanf("%d", &valor);
        matriz[fila_actual][col_actual] = valor;

        pthread_mutex_unlock(&lock_coordinador);
    }

    return NULL;
}

void *negativo(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&lock_negativo);

        if (fila_actual < 0)
        {
            // Aquí en vez de salir, se bloquea indefinidamente para seguir "vivo"
            while (1)
            {
                sleep(10);
            }
        }

        int valor;
        printf("[NEGATIVO] Ingresa un número para [%d][%d]: ", fila_actual, col_actual);
        scanf("%d", &valor);
        matriz[fila_actual][col_actual] = valor;

        pthread_mutex_unlock(&lock_coordinador);
    }

    return NULL;
}