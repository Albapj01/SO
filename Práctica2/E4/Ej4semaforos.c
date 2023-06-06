#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

int vglobal = 0;
pthread_mutex_t sem;

void funcionHilo()
{
	printf("Soy el hilo %lu \n", pthread_self());
	for(int i=0; i<10000; i++)
	{
		pthread_mutex_lock(&sem);
		vglobal++;
		pthread_mutex_unlock(&sem);
	}
	printf("La variable global tras el incremento es %d \n",vglobal);
}

int main(int argc, char ** argv)
{

	pthread_t id_hilos[2];
	pthread_mutex_init(&sem, NULL);

	for (int i = 0; i < 2; ++i)
	{
		if(pthread_create(&id_hilos[i], NULL, (void*)funcionHilo, NULL) != 0)
		{
			printf("Error");
			exit(EXIT_FAILURE);
		}	
	}

	//ESPERA DEL HILO
	for (int i = 0; i < 2; ++i)
	{
		if(pthread_join(id_hilos[i], (void**)NULL) != 0) //hace la ejecucion de la funcion y el control de errores
		{
			printf("Error");
			exit(EXIT_FAILURE);
		}
	}
	pthread_mutex_destroy(&sem);
}
