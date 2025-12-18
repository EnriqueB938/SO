
//Saber obligatoriamente sobre MP saber contar lineas de un fichero, saber si un numero es primo y el factorial.

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>


struct param
{
pthread_mutex_t *semaforo;
int numero_random;
int *compartida;
};


void funcionHiloSumador(void *parametro)
{
	struct param p = *(struct param *)parametro;

	srand(time(NULL));
}

void funcionHiloRestador(void *parametro)
{
	struct param p = *(struct param *)parametro;

	srand(time(NULL));
}

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		printf("Error en la linea de argumentos\n");
		exit(EXIT_FAILURE);
	}

	int n1 = atoi(argv[1]);
	int n2 = atoi(argv[2]);
	pthread_t sumador, restador;
	pthread_mutex_t semaforo;
	struct param p;
	int numero_random;
	int compartida = 0;

	p.semaforo = &semaforo;
	p.compartida = &compartida;
	p.numero_random = numero_random;

	if(pthread_mutex_init(&semaforo, NULL) != 0)
	{
		printf("Error en el init()\n");
		exit(EXIT_FAILURE);
	}

	if(pthread_create(&sumador, NULL, (void*)funcionHiloSumador, (void**)&p) != 0)
	{
		printf("Error en el create()\n");
		exit(EXIT_FAILURE);
	}

	if(pthread_create(&restador, NULL, (void*)funcionHiloRestador, (void**)&p) != 0)
	{
		printf("Error en el create()\n");
		exit(EXIT_FAILURE);
	}

	void *valorRecogido1;
	void *valorRecogido2;

	if(pthread_join(funcionHiloSumador, (void*)&valorRecogido1) !=0)
	{
		printf("Error en el join()\n");
		exit(EXIT_FAILURE);
	}

	if(pthread_join(funcionHiloRestador, (void*)&valorRecogido2) !=0)
	{
		printf("Error en el join()\n");
		exit(EXIT_FAILURE);
	}

	if(pthread_mutex_destroy(&semaforo) !=0)
	{
		printf("Error en el destroy()\n");
		exit(EXIT_FAILURE);
	}

	free(valorRecogido1);
	free(valorRecogido2);

	exit(EXIT_SUCCESS);

}