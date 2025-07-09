#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#define ROWS 4
#define COLS 3

int matriz[ROWS][COLS];
int fila_actual = 0, col_actual = 0;

pthread_t hilo_coordinador, hilo_positivo, hilo_negativo;

volatile sig_atomic_t senal_recibida = 0;

// Manejadores de señales usando signal()
void handler(int signum)
{
    senal_recibida = signum;
}

void esperar_senal(int sig_esperada)
{
    senal_recibida = 0;
    while (senal_recibida != sig_esperada)
    {
        pause();
    }
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

            if (producto % 2 == 0)
            {
                // Producto par o cero -> hilo positivo
                pthread_kill(hilo_positivo, SIGUSR1);
            }
            else
            {
                // Producto impar -> hilo negativo
                pthread_kill(hilo_negativo, SIGUSR2);
            }

            // Esperar señal SIGALRM de hilo positivo o negativo
            esperar_senal(SIGALRM);
        }
    }

    // Mostrar matriz
    printf("\n--- Matriz final ---\n");
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
    }

    // Orden de finalización
    pthread_kill(hilo_negativo, SIGTERM);
    esperar_senal(SIGALRM);

    pthread_kill(hilo_positivo, SIGTERM);
    esperar_senal(SIGALRM);

    pthread_exit(NULL);
}

void *positivo(void *arg)
{
    signal(SIGUSR1, handler);
    signal(SIGTERM, handler);

    while (1)
    {
        esperar_senal(SIGUSR1);

        if (senal_recibida == SIGTERM)
            break;

        int valor;
        printf("[POSITIVO] Ingresa valor para [%d][%d]: ", fila_actual, col_actual);
        scanf("%d", &valor);
        matriz[fila_actual][col_actual] = valor;

        // Avisar coordinador que terminó
        pthread_kill(hilo_coordinador, SIGALRM);
    }

    pthread_kill(hilo_coordinador, SIGALRM); // Confirmar terminación
    pthread_exit(NULL);
}

void *negativo(void *arg)
{
    signal(SIGUSR2, handler);
    signal(SIGTERM, handler);

    while (1)
    {
        esperar_senal(SIGUSR2);

        if (senal_recibida == SIGTERM)
            break;

        int valor;
        printf("[NEGATIVO] Ingresa valor para [%d][%d]: ", fila_actual, col_actual);
        scanf("%d", &valor);
        matriz[fila_actual][col_actual] = valor;

        // Avisar coordinador que terminó
        pthread_kill(hilo_coordinador, SIGALRM);
    }

    pthread_kill(hilo_coordinador, SIGALRM); // Confirmar terminación
    pthread_exit(NULL);
}

int main()
{
    // Registrar manejadores básicos para señales
    signal(SIGALRM, handler);

    if (pthread_create(&hilo_positivo, NULL, positivo, NULL) != 0)
    {
        perror("Error creando hilo positivo");
        exit(1);
    }
    if (pthread_create(&hilo_negativo, NULL, negativo, NULL) != 0)
    {
        perror("Error creando hilo negativo");
        exit(1);
    }
    if (pthread_create(&hilo_coordinador, NULL, coordinador, NULL) != 0)
    {
        perror("Error creando hilo coordinador");
        exit(1);
    }

    pthread_join(hilo_coordinador, NULL);
    pthread_join(hilo_positivo, NULL);
    pthread_join(hilo_negativo, NULL);

    printf("Programa terminado correctamente.\n");
    return 0;
}
