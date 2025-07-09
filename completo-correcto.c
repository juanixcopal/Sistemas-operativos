#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#define ROWS 4
#define COLS 3

// Matriz y pisición actual
int matriz[ROWS][COLS];
int fila_actual = 0, col_actual = 0;

// Hilos globales
pthread_t hilo_coordinador, hilo_positivo, hilo_negativo;

// Flag para saber que señal se recibió
volatile sig_atomic_t senal_recibida = 0;

void *coordinador(void *args);
void *positivo(void *args);
void *negativo(void *args);

// === Handler general ===
void handler(int sig)
{
    senal_recibida = sig;
}

// Esperar señal especifica (usando pause)
void esperar_senal(int esperada)
{
    while (1)
    {
        pause();
        if (senal_recibida == esperada)
            return;
    }
}

int main()
{
    // Registrar SIGALRM en main por seguridad
    signal(SIGALRM, handler);

    // Crear hilos
    pthread_create(&hilo_positivo, NULL, positivo, NULL);
    pthread_create(&hilo_negativo, NULL, negativo, NULL);
    pthread_create(&hilo_coordinador, NULL, coordinador, NULL);

    // h) Terminar el programa solo cuando todos los hilos terminen
    pthread_join(hilo_coordinador, NULL);
    pthread_join(hilo_positivo, NULL);
    pthread_join(hilo_negativo, NULL);

    printf("Programa finalizado correctamente.\n");
    return 0;
}

void *coordinador(void *args)
{
    // Asignar handler para SIGALRM (respuesta de positivo o negativo)
    signal(SIGALRM, handler);

    // Llenar matriz
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            fila_actual = i;
            col_actual = j;

            int producto = i * j;

            if (producto % 2 == 0)
            {
                pthread_kill(hilo_positivo, SIGUSR1);
            }
            else
            {
                pthread_kill(hilo_negativo, SIGUSR2);
            }

            esperar_senal(SIGALRM);
        }
    }

    // f) Mostrar matriz
    printf("\nMatriz final:\n");
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
    }

    // Actualizar el valor de fila_actual para que los hilos detecten que deben de salir
    fila_actual = ROWS;

    // g) Finalización ordenada
    pthread_kill(hilo_negativo, SIGUSR2);
    esperar_senal(SIGALRM);

    pthread_kill(hilo_positivo, SIGUSR1);
    esperar_senal(SIGALRM);

    printf("Coordinador finaliza \n");
    pthread_exit(NULL);
}

void *positivo(void *args)
{
    signal(SIGUSR1, handler);

    while (1)
    {
        esperar_senal(SIGUSR1);

        // h) Si la matriz ya está completa, salir
        if (fila_actual >= ROWS)
        {
            printf("Hilo POSITIVO finaliza.\n");
            pthread_kill(hilo_coordinador, SIGALRM);
            pthread_exit(NULL);
        }

        int valor;
        printf("[POSITIVO] Ingresa un número para [%d][%d]: ", fila_actual, col_actual);
        scanf("%d", &valor);
        matriz[fila_actual][col_actual] = valor;

        pthread_kill(hilo_coordinador, SIGALRM);
    }

    return NULL;
}

void *negativo(void *args)
{
    signal(SIGUSR2, handler);

    while (1)
    {
        esperar_senal(SIGUSR2);

        // h) Si la matriz ya está completa, salir
        if (fila_actual >= ROWS)
        {
            printf("Hilo NEGATIVO finaliza.\n");
            pthread_kill(hilo_coordinador, SIGALRM);
            pthread_exit(NULL);
        }

        int valor;
        printf("[NEGATIVO] Ingresa un número para [%d][%d]: ", fila_actual, col_actual);
        scanf("%d", &valor);
        matriz[fila_actual][col_actual] = valor;

        pthread_kill(hilo_coordinador, SIGALRM);
    }

    return NULL;
}