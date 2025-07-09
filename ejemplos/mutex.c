/* mutex.c mutex protege zonas criticas, aqui se crea 2 hilos*/
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int contador = 0;

void *incrementa(void *arg)
{
    for (int i = 0; i < 1000; ++i)
    {
        pthread_mutex_lock(&m); // bloquea el acceso. Solo 1 hilo puede entrar
        ++contador;
        pthread_mutex_unlock(&m); // libera el acceso para que otro hilo pueda entrar
    }
    return NULL;
}

int main()
{
    pthread_t h1, h2;
    pthread_create(&h1, NULL, incrementa, NULL);
    pthread_create(&h2, NULL, incrementa, NULL);
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    printf("Valor final = %d\n", contador); /* Debe ser 2000 */
    return 0;
}
