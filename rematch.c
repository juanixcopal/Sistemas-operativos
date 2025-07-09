#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>
#include <sys/wait.h>

void *pares();
void *impares();
int calcular_factorial(int n);

int numeros[5];
long factoriales[5];
int max_random = 20;

pthread_mutex_t lock;

int main()
{
    int tuberia[2];
    pid_t hijo;

    pthread_t hilo_pares, hilo_impares;

    // Inicializar tubería
    if (pipe(tuberia) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Generar números aleatorios
    printf("[Padre] Generando números aleatorios:\n");

    srand(time(NULL));

    for (int i = 0; i < 5; i++)
    {
        numeros[i] = (rand() % max_random) + 1;
        printf("[Padre] Número %d: %d\n", i, numeros[i]);
    }

    hijo = fork();

    if (hijo == 0)
    {
        // Hijo
        close(tuberia[0]); // Cierra lectura

        printf("[Hijo] Proceso hijo iniciado (PID: %d)\n", getpid());

        pthread_mutex_init(&lock, NULL);

        pthread_create(&hilo_pares, NULL, pares, NULL);
        pthread_create(&hilo_impares, NULL, impares, NULL);

        pthread_join(hilo_pares, NULL);
        pthread_join(hilo_impares, NULL);

        pthread_mutex_destroy(&lock);

        // Enviar los factoriales al padre
        for (int i = 0; i < 5; i++)
        {
            write(tuberia[1], &factoriales[i], sizeof(long));
        }

        close(tuberia[1]);
        exit(0);
    }
    else
    {
        // Padre
        close(tuberia[1]); // Cierra escritura

        wait(NULL); // Espera al hijo

        printf("[Padre] Recibiendo resultados del hijo:\n");
        for (int i = 0; i < 5; i++)
        {
            read(tuberia[0], &factoriales[i], sizeof(long));
            printf("[Padre] El factorial de %d (posición %d) = %ld\n", numeros[i], i, factoriales[i]);
        }

        close(tuberia[0]);
    }

    return 0;
}

void *pares()
{
    // Aqui debe de procesar los numeros pares del array
    // numeros 0, 2 y 4
    printf("[Hilo 1] Procesando numeros en posiciones pares: \n");
    for (int par = 0; par < 5; par += 2)
    {
        long fact = calcular_factorial(numeros[par]);
        pthread_mutex_lock(&lock);
        factoriales[par] = fact;
        pthread_mutex_unlock(&lock);
        printf("[Hilo 1] Factorial de %d (posición %d) = %ld\n", numeros[par], par, fact);
    }
    return NULL;
}

void *impares()
{
    // Aqui debe de procesar los numeros impares del array
    // numeros 1 y 3
    printf("[Hilo 2] Procesando numeros en posiciones impares: \n");
    for (int impar = 1; impar < 5; impar += 2)
    {
        long fact = calcular_factorial(numeros[impar]);
        pthread_mutex_lock(&lock);
        factoriales[impar] = fact;
        pthread_mutex_unlock(&lock);
        printf("[Hilo 2] Factorial de %d (posición %d) = %ld\n", numeros[impar], impar, fact);
    }
    return NULL;
}

// Función para calcular factorial
int calcular_factorial(int n)
{
    long producto = 1;
    for (int i = 2; i <= n; i++)
    {
        producto *= i;
    }
    return producto;
}