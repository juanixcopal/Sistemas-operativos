/* senal_hilos.c */
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

pthread_t receptor;

void handler(int s) { printf("Hilo recibió señal %d\n", s); }

void *espera(void *p)
{
    signal(SIGUSR1, handler); // Asigna el handler para SIGUSR1
    pause();                  // Detiene el hilo aquí hasta que reciba una señal
    return NULL;              // Cuando recibe la señal, termina
}

int main()
{
    pthread_create(&receptor, NULL, espera, NULL);
    sleep(1);
    pthread_kill(receptor, SIGUSR1); // Aquí el hilo principal (main) envía una señal SIGUSR1 al hilo receptor
    pthread_join(receptor, NULL);    // Finalmente, el programa espera que el hilo receptor termine (pthread_join) y luego finaliza.
    return 0;
}
