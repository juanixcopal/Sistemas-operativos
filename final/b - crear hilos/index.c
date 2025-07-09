// Crea 3 hilos que se llamaran coordinador positivo negativo.
// Estos hilos deben permanecer vivos hasta el final del programa

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

void *coordinador(void *arg);
void *positivo(void *arg);
void *negativo(void *arg);

int main(void)
{
    pthread_t hilo_coordinador, hilo_positivo, hilo_negativo;

    pthread_create(&hilo_coordinador, NULL, coordinador, NULL);
    pthread_create(&hilo_positivo, NULL, positivo, NULL);
    pthread_create(&hilo_negativo, NULL, negativo, NULL);

    pthread_join(hilo_coordinador, NULL);
    pthread_join(hilo_positivo, NULL);
    pthread_join(hilo_negativo, NULL);

    return 0;
}

void *coordinador(void *arg)
{
    while (1)
    {
        printf("Hilo coordinador activo\n");
        sleep(2);
    }

    return NULL;
}

void *positivo(void *arg)
{
    while (1)
    {
        printf("Hilo positivo activo\n");
        sleep(2);
    }

    return NULL;
}

void *negativo(void *arg)
{
    while (1)
    {
        printf("Hilo negativo activo\n");
        sleep(2);
    }

    return NULL;
}