#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

int v[5]={20,20,20,20,20};

sem_t s;
sem_t n;
sem_t e;

void productores()
{
	int m;
	for(int i=0; i<10; i++)
	{
		
		sem_wait(&e);
		sem_wait(&s);

		m = rand()%10;

		v[m] = v[m]+ rand()%10;

		sem_post(&s);
		sem_post(&n);
	}
}

void consumidores()
{
	int m;
	for(int i=0; i<10; i++)
	{
		sem_wait(&n);
		sem_wait(&s);

		m = rand()%10;

		v[m] = v[m]- rand()%10;

		sem_post(&s);
		sem_post(&e);
	}
}

int main()
{

	srand(time(NULL));

	sem_init(&s, 0, 1);
	sem_init(&n, 0, 0);
	sem_init(&e, 0, 5);

	pthread_t id_productor;

	
		if(pthread_create(&id_productor, NULL, (void*)productores, NULL) != 0)
		{
			printf("Error");
			exit(EXIT_FAILURE);
		}	
	

	pthread_t id_consumidor;

	
		if(pthread_create(&id_consumidor, NULL, (void*)consumidores, NULL) != 0)
		{
			printf("Error");
			exit(EXIT_FAILURE);
		}	



		if(pthread_join(id_productor, (void**)NULL) != 0)
		{
			printf("Error");
			exit(EXIT_FAILURE);
		}


	
		if(pthread_join(id_consumidor, (void**)NULL) != 0)
		{
			printf("Error");
			exit(EXIT_FAILURE);
		}
	

	sem_destroy(&s);
	sem_destroy(&n);
	sem_destroy(&e);

	exit(EXIT_SUCCESS);
}
