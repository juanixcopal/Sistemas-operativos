/* hilo_basico.c */
#include <stdio.h>
#include <pthread.h>

void *uno(void *arg)
{
    puts("¡Hola desde un hilo!");
    return NULL;
}

int main()
{
    pthread_t hilo_uno;
    pthread_create(&hilo_uno, NULL, uno, NULL);
    pthread_join(hilo_uno, NULL);
    return 0;
}
