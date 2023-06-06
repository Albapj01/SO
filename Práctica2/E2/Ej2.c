#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int contarLineasFichero(char * nombreFichero)
{
	int cont = 0;
	FILE * fich = fopen(nombreFichero, "r");
	if(fich==NULL)
	{
		printf("Error al abrir el fichero\n");
		exit(EXIT_FAILURE);
	} 
	else
	{
		char linea [200];
		while(fgets(linea, 200, fich))
		{
			cont++;
		}
		fclose(fich);
	}
	return cont;
}

int main(int argc, char ** argv)
{
	if(argc==1)
	{
		printf("Error en linea de argumentos \n");
		exit(EXIT_FAILURE);
	}

	pid_t pid;
	pid_t flag;
	int status;
	int suma = 0, cont=0;

	//1) CREACIÓN (PROCESO DE CLONACIÓN)
	//2) GESTIÓN
	printf("Soy el padre, mi pid es %ld y el de mi padre es %ld.\n",(long int)getpid(),(long int)getppid());
	for (int i = 0; i < argc - 1; ++i)
	{
		
		pid = fork();
		if(pid==-1)
		{
			printf("ERROR en el fork()\n");		
			exit(EXIT_FAILURE);
		}
		else if(pid == 0)//HIJO1
		{
			printf("Soy el hijo, mi pid es %ld y el de mi padre es %ld.\n",(long int)getpid(),(long int)getppid());
			
			int cont = contarLineasFichero(argv[i+1]);

			printf("Soy el proceso %ld, mi padre es %ld y el fichero %s tiene %d lineas.\n",(long int)getpid(),(long int)getppid(),argv[i+1],cont);

			exit(cont);
		}
	}
	


	//3) ESPERA (bloqueante)
	while ( (flag=wait(&status)) > 0 ) 
	{
		if (WIFEXITED(status)) {
			printf("Proceso Padre, Hijo con PID %ld finalizado, status = %d\n", (long int)flag, WEXITSTATUS(status));
		} 
		else if (WIFSIGNALED(status)) {  //Para seniales como las de finalizar o matar
			printf("Proceso Padre, Hijo con PID %ld finalizado al recibir la señal %d\n", (long int)flag, WTERMSIG(status));
		} 
		else if (WIFSTOPPED(status)) { //Para cuando se para un proceso. Al usar wait() en vez de waitpid() no nos sirve.
			printf("Proceso Padre, Hijo con PID %ld parado al recibir la señal %d\n", (long int)flag, WSTOPSIG(status));
		} 
		else if (WIFCONTINUED(status)){ //Para cuando se reanuda un proceso parado. Al usar wait() en vez de waitpid() no nos sirve.
			printf("Proceso Padre, Hijo con PID %ld reanudado\n",(long int) flag);		  
		}	
		suma = suma + WEXITSTATUS(status);
	}
	if (flag==(pid_t)-1 && errno==ECHILD)
	{
		printf("Proceso Padre, valor de errno = %d, definido como: %s\n", errno, strerror(errno));
	}
	else
	{
		printf("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal.\n");
		exit(EXIT_FAILURE);
	}
	
	
	printf("La suma de lineas es %d\n", suma);
	

	exit(suma); 
}