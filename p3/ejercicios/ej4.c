/*
Una tienda que vende camisetas guarda en una base de datos (buffer) las cantidades de camisetas
que tiene según el modelo. Por ejemplo, un buffer de camisetas[5] indica que existen 5 modelos de camisetas
y cada elemento de este buffer guarda las cantidades iniciales de cada una de ellas. 
Implementar un programa que genere N clientes (pedido por linea de argumentos) y M proveedores
(la misma cantidad de proveedores que modelos de camiseta, también pedido por linea de argumentos).
Para simular una compra, cada hilo Cliente debe generar un valor aleatorio para el modelo de camiseta que quiere comprar
y otro para la cantidad a comprar. Esta cantidad debe decrementar el stock de la camiseta en cuestión.
Para simular un suministro, cada Proveedor debe hacer lo mismo que el Cliente pero en este caso,
incrementando el stock de la camiseta en cuestión. Utilice semáforos binarios para resolver este problema
de concurrencia imprimiendo el buffer antes de generar los hilos y al final del programa para comprobar
que se ha ejecutado correctamente. En cuanto al numero de camisetas a comprar o suministrar,
por ejemplo pueden ser valores aleatorios entre [1-10]. Para rellenar el vector inicial de stock de camisetas
podría generar números aleatorios entre [1-100]. Si se diera el caso de que algún cliente quiere comprar
más camisetas de las disponibles solo se le proporcionarán las que queden.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

struct param
{
    int *vector;
    pthread_mutex_t *semaforo; //Variables compartidas usamos *
    int M;
};

void funcionHiloCompra(void *parametro)
{

}

void funcionHiloSuministro(void *parametro)
{

}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Error en línea de argumentos...\n");
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int *vector = (int *)malloc(sizeof(int));
    pthread_mutex_t semaforo;
    pthread_t clientes[N];
    pthread_t porveedores[M];
    struct param p;
    srand(time(NULL));

    p.semaforo = &semaforo;
    p.vector = vector;
    p.M = M;

    if(vector == NULL)
    {
        printf("ERROR... al reservar memoria\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<M; i++)
    {
        vector[i] = rand() % 100 + 1;
    }

    for(int i=0, i<M, i++)
    {
        printf("%d", vector[i]);
    }

    if (pthread_mutex_init(&semaforo, NULL) != 0)
    {
        printf("Error en pthread_mutex_init(semaforo)\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < N; i++)
    {
        if (pthread_create(&clientes[i], NULL, (void *)funcionHiloCompra, (void *)&p) != 0)
        {
            printf("Error en pthread_create().\n");
            exit(EXIT_FAILURE);
        }
    }

        for (size_t i = 0; i < M; i++)
    {
        if (pthread_create(&proveedores[i], NULL, (void *)funcionHiloSuministro, (void *)&p) != 0)
        {
            printf("Error en pthread_create().\n");
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i = 0; i < N; i++)
    {
        if (pthread_join(clientes[i], (void **)NULL) != 0)
        {
            printf("Error en pthread_join()\n");
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i = 0; i < M; i++)
    {
        if (pthread_join(proveedores[i], (void **)NULL) != 0)
        {
            printf("Error en pthread_join()\n");
            exit(EXIT_FAILURE);
        }
    }

    for(int i=0, i<M, i++)
    {
        printf("%d", vector[i]);
    }

    if (pthread_mutex_destroy(&semaforo) != 0)
    {
        printf("Error en pthread_mutex_destroy(semaforo)\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}