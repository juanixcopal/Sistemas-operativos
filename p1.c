#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>

void *pares();
void *impares();
int calcular_factorial(int n);

pthread_t hilo_pares, hilo_impares;
pthread_mutex_t lock_pares, lock_impares;

int numeros [5];
int max_random=20;
int long factoriales [5];

int main(void){
    printf("[Padre] Generando números aleatorios: \n");
    
    srand(time(NULL));

    for(int i = 0; i < 5; i++)
    {
        numeros[i] = (rand() % max_random) + 1;
        printf("[Padre]: Numero %d : %d\n", i, numeros[i]);
    }
    
    pid_t hijo;

    hijo=fork();

    if(hijo == 0){
        printf("[Hijo] Proceso hijo iniciado (PID: %d ) \n", getpid());

        pthread_create(&hilo_pares, NULL, &pares, NULL);
        pthread_create(&hilo_impares, NULL, &impares, NULL);

        pthread_join(hilo_pares, NULL);
        pthread_join(hilo_impares, NULL);

        pthread_mutex_destroy(&lock_pares);
        pthread_mutex_destroy(&lock_impares);
    }
}


void *pares(){
    //Aqui debe de procesar los numeros pares del array
    //numeros 0, 2 y 4
    printf("[Hilo 1] Procesando numeros en posiciones pares: \n");
    pthread_mutex_lock(&lock_pares);

    int fact;

    for(int par = 0; par < 5; par ++){
        if(numeros[par] % 2 == 0){
            fact = calcular_factorial(numeros[par]);
            factoriales[par] = fact;
            printf("Factorial de %d (posicion %d) = %d \n", numeros[par], par, fact);
        }
    }

    pthread_mutex_unlock(&lock_pares);
}

void *impares(int size, int *impares[]){
    //Aqui debe de procesar los numeros impares del array
    //numeros 1 y 3
}

int calcular_factorial(int n){
    int producto = 1;
    for (int i = n; i > 1; --i) {
        producto *= i;
    }

    return (producto);
}