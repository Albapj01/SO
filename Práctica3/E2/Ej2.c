#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int par = 0;
int impar = 0;
pthread_mutex_t sem;
pthread_mutex_t sem1;

void funcionHilo(int * n)
{

	int suma = 0;

	for(int i=0; i<5; i++)
	{
		suma = suma+rand()%11;
	}

	if((*n)%2==0)
	{
		pthread_mutex_lock(&sem);
		par = par + suma;
		pthread_mutex_unlock(&sem);	
	}
	else
	{
		pthread_mutex_lock(&sem1);
		impar = impar + suma;
		pthread_mutex_unlock(&sem1);	
	}
	
	int * retorno = (int*)malloc(sizeof(int)); //se añade siempre para retornar un valor en la funcion del hilo
	(*retorno)=suma;
	pthread_exit((void*)retorno);
}



int main(int argc, char ** argv)
{
	if(argc==1)
	{
		printf("Error en linea de argumentos \n");
		exit(EXIT_FAILURE);
	}

	srand(time(NULL));

	int n = atoi(argv[1]);

	pthread_mutex_init(&sem, NULL);
	pthread_mutex_init(&sem1, NULL);

	pthread_t id_hilos[n];

	int posiciones[n];

	for (int i = 0; i < n; ++i)
	{
		posiciones[i]=i+1;
		if(pthread_create(&id_hilos[i], NULL, (void*)funcionHilo, &posiciones[i]) != 0) //&posiciones, es el 1, 2, 3 que recibe la funcion del hilo
		{
			printf("Error");
			exit(EXIT_FAILURE);
		}	
	}

	int retorno_pares = 0, retorno_impares = 0;
	int * retorno;

	for (int i = 0; i < n; ++i)
	{
		if(pthread_join(id_hilos[i], (void**)&retorno) != 0) //retorno recoge el valor que devuelve el hilo
		{
			printf("Error");
			exit(EXIT_FAILURE);
		}
		if((i+1)%2==0) //en el create hemos empezado en 1
		{
			retorno_pares = retorno_pares + (*retorno); 
		}
		else
		{
			retorno_impares = retorno_impares + (*retorno);
		}
		printf("Main()... La hebra de orden de creacion %d devolvio el valor de suma total: %d\n", posiciones[i], (*retorno));
		free(retorno);
	}
	
	printf("\nValor de la variable compartida impar: %d\n", impar);
	printf("Suma de los valores del vueltos por las hebras de creacion impares %d\n", retorno_impares);

	printf("\nValor de la variable compartida par: %d\n", par);
	printf("Suma de los valores del vueltos por las hebras de creacion pares %d\n", retorno_pares);

	pthread_mutex_destroy(&sem);
	pthread_mutex_destroy(&sem1);

	exit(EXIT_SUCCESS);
}
