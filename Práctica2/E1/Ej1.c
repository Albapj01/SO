#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


void imprimirCadena(char * cadena)
{
	for (int i = 0; i < strlen(cadena); ++i)
	{
		printf("%c \n", cadena[i]);
		sleep(1);
	}
}

int main()
{
	pthread_t id_hilo1, id_hilo2;

	char cadena1[]="Hola";
	char cadena2[]="Mundo";

	if(pthread_create(&id_hilo1, NULL, (void *)imprimirCadena, cadena1) != 0)
	{
		printf("Error");
		exit(EXIT_FAILURE);
	}

	if(pthread_create(&id_hilo2, NULL, (void *)imprimirCadena, cadena2) != 0)
	{
		printf("Error");
		exit(EXIT_FAILURE);
	}


	if(pthread_join(id_hilo1, (void **)NULL) != 0)
	{
		printf("Error");
		exit(EXIT_FAILURE);
	}

	if(pthread_join(id_hilo2, (void **)NULL) != 0)
	{
		printf("Error");
		exit(EXIT_FAILURE);
	}
	
	exit(EXIT_SUCCESS);


}
