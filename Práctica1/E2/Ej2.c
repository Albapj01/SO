#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


int main()
{
  pid_t pid, childpid; 
  int status; 

  printf("Proceso padre con pid %d\n", getpid());
  pid = fork(); 

  switch(pid) 
  {
    case 0:
      	printf("Hijo: Mi pid es %d. El pid de mi padre es %d\n", getpid(), getppid()); 
      	exit(EXIT_SUCCESS); //Muere el hijo

    case -1: 
      	printf("Error, no se ha podido crear al proceso hijo\n"); 
      	exit(EXIT_FAILURE);  

    default: 
        printf("Padre: Voy a esperar a mi hijo\n");
        sleep(20);
      while ( (childpid=waitpid(-1,&status,WUNTRACED | WCONTINUED)) > 0 )
	    {
	    	if (WIFEXITED(status)) //En caso de que el proceso del hijo termine de forma correcta, entramos en este if debido a que la función WIFEXITED() devuelve un valor distinto de 0 (en este caso) true cuando esto sucede
	    	{
	    		printf("Proceso padre %d, hijo con PID %ld finalizado, status = %d\n", getpid(), (long int)childpid, WEXITSTATUS(status)); // usamos WEXITSTATUS(status) para imprimir el estado del hijo
	    	} 
	    	else if (WIFSIGNALED(status)) //En caso de que el proceso del hijo haya concluido a causa de una señal no capturada entramos en el else if ya que WIFSIGNALED() devuelve un valor distinto de 0 (en este caso true), cuando esto sucede
	    	{
	    		printf("Proceso padre %d, hijo con PID %ld finalizado al recibir la señal %d\n", getpid(), (long int)childpid, WTERMSIG(status)); //usamos WTERMSIG(status) cuando WIFSIGNALED() es distinto de 0 (en este caso true) y devuelve  el  número de la señal que ha provocado la muerte del proceso hijo
	    	}
	    }

	    if (childpid==(pid_t)-1 && errno==ECHILD) //Como aquí entramos cuando no tenemos que esperar ningun hijo mas, el valor de errno es 10
	    {
	    	printf("Proceso padre %d, no hay mas hijos que esperar. Valor de errno = %d, definido como: %s\n", getpid(), errno, strerror(errno));
	    }	
	    else
	    {
	    	printf("Error en la invocacion de wait o waitpid. Valor de errno = %d, definido como: %s\n", errno, strerror(errno));
	    	exit(EXIT_FAILURE);
	    } 

    sleep(20);
    exit(EXIT_SUCCESS); 
  }
}