#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


//int cont =0;

void tratarSennal(int signal)
{
	printf("Soy el hijo %ld y he recibido una señal de mi padre %ld\n", (long int)getpid(), (long int)getppid());
	//cont++;
}

int main()
{

	pid_t pid;
	pid_t flag;
	int status;

	//1) CREACIÓN (PROCESO DE CLONACIÓN)
	
	pid = fork();

	//2) GESTIÓN
	if(pid==-1)
	{
		printf("ERROR en el fork()\n");		
		exit(EXIT_FAILURE);
	}
	else if(pid == 0)//HIJO1
	{
		printf("Soy el hijo, mi pid es %ld y el de mi padre es %ld.\n",(long int)getpid(),(long int)getppid());

		signal(SIGUSR1, &tratarSennal);
		while(1); //while(cont<5) y se añade exit(EXIT_SUCCESS)
	}
	else
	{
		printf("Soy el padre, mi pid es %ld y el de mi padre es %ld.\n",(long int)getpid(),(long int)getppid());
		for (int i = 0; i < 5; ++i)
		{
			sleep(1);
			kill(pid, SIGUSR1);
		}
		sleep(1); //se quita este para el cont
		kill(pid, SIGKILL); //se quita este para el cont
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


	exit(EXIT_SUCCESS); 
}