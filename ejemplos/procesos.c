/* fork_wait.c */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid1 = fork();
    if (pid1 == 0)
    { /* hijo 1 */
        puts("Soy el hijo 1");
        _exit(0);
    }
    else
    {               /* padre */
        wait(NULL); // espera al hijo 1

        pid_t pid2 = fork();
        if (pid2 == 0)
        { /* hijo 2 */
            puts("Soy el hijo 2");
            _exit(0);
        }
        else
        {               /* padre */
            wait(NULL); // espera al hijo 2
            puts("Los hijos terminaron");
        }
    }
    return 0;
}
