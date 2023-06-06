#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char ** argv)
{
	if(argc==1)
	{
		printf("Error linea argumentos \n");
		exit(EXIT_FAILURE);
	}
	int n = atoi(argv[1]);

	int fact = 1;
	for (int i = 1; i <= n; ++i)
	{
		printf("%d x %d = %d\n",fact,i,fact*i);
		fact = fact * i;
		sleep(1);
	}
	printf("%d \n",fact);

	return(0);
}