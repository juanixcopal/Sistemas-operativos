#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h>

// static int *variable_compartida;

int main (void)
{
	pid_t hijo;
	int *variable_compartida;
	variable_compartida=mmap(NULL, sizeof *variable_compartida, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*variable_compartida=0;
	hijo=fork();
	if (hijo==0)//PRoceso hijo
	{
		puts("Soy el hijo, dame un valor para la variable compartida");
		scanf("%d", *&variable_compartida);
		exit(0);
	}
	else if (hijo>0)//Proceso padre
	{
		puts("Soy el proceso padre, quedo a la espera del que el hijo acabe");
		wait (NULL);
		printf("Soy el padre, y el hijo ha cambiado el valor de la variable a %d\n", *variable_compartida);
	}
	munmap(variable_compartida, sizeof(variable_compartida));
	return 0;		
}

