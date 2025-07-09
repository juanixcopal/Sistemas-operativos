/* semaforo.c  (compilar: gcc semaforo.c -pthread -lrt) */
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h> /* O_CREAT */
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    // Crea o abre un semáforo POSIX con nombre "/demo_sem"
    // O_CREAT → lo crea si no existe
    // 0600 → permisos (lectura/escritura solo para el dueño)
    // 0 → valor inicial del semáforo (cerrado/bloqueado)
    sem_t *sem = sem_open("/demo_sem", O_CREAT, 0600, 0);

    if (fork() == 0)
    {             // Proceso hijo
        sleep(1); // Espera 1 segundo para que el padre llegue al sem_wait primero
        puts("Hijo: libera semáforo");

        // Incrementa el semáforo (lo libera), permitiendo que el padre continúe
        sem_post(sem);

        // Termina el hijo
        _exit(0);
    }
    else
    { // Proceso padre
        puts("Padre: esperando semáforo");

        // Espera (bloquea) hasta que otro proceso llame a sem_post
        sem_wait(sem);

        // Continúa una vez que el semáforo es liberado por el hijo
        puts("Padre: recibido!");

        // Cierra el descriptor del semáforo
        sem_close(sem);

        // Elimina el semáforo del sistema (ya no estará disponible con ese nombre)
        sem_unlink("/demo_sem");
    }

    return 0;
}