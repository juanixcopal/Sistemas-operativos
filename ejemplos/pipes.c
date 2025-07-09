/* pipe_demo.c */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int fd[2];
    pipe(fd); /* fd[0]=lectura, fd[1]=escritura */

    pid_t proceso1 = fork();

    if (proceso1 == 0)
    {                 /* hijo: lee */
        close(fd[1]); // Cierra escritura
        char buf[64];
        read(fd[0], buf, sizeof(buf));
        printf("Hijo leyó: %s\n", buf);
    }
    else
    {                 /* padre: escribe */
        close(fd[0]); // Cierra lectura
        const char *msg = "Hola desde el padre\n";
        write(fd[1], msg, strlen(msg) + 1);
        wait(NULL);
    }
    return 0;
}
