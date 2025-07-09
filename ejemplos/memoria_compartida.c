/* mmap_shared.c */
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    // Crear una región de memoria compartida anónima de tamaño 'int'
    // PROT_READ | PROT_WRITE → se puede leer y escribir
    // MAP_SHARED → se comparte entre procesos (padre e hijo)
    // MAP_ANONYMOUS → no está asociada a ningún archivo (no es un mapeo de archivo)
    // -1 y 0 → como es anónima, no se necesita descriptor de archivo ni offset
    int *compartido = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // Inicializa el valor compartido en 0
    *compartido = 0;

    // Crear un nuevo proceso
    pid_t proceso1 = fork();

    if (proceso1 == 0)
    {
        // Código que ejecuta el hijo
        // Escribe el valor 42 en la memoria compartida
        (*compartido) = 42;

        // Termina el proceso hijo sin pasar por el buffer de salida (evita conflictos con printf)
        _exit(0);
    }
    else
    {
        // Código que ejecuta el padre

        // Espera a que el hijo termine
        wait(NULL);

        // Imprime el valor que el hijo escribió en la memoria compartida
        printf("Valor en padre = %d\n", *compartido); // Se espera: 42
    }

    return 0;
}
