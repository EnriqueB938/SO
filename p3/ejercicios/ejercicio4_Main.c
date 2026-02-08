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
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

struct param
{
    pthread_mutex_t *semaforo;
    int *vector;
    int M;
};

void *simularCompra(void *parametro)
{
    struct param p = *(struct param *)parametro;
    int posicion = rand() % (p.M);
    int cantidad = (rand() % 10) + 1;

    if (pthread_mutex_lock(p.semaforo) != 0)
    {
        printf("Error en pthread_mutex_lock()\n");
        exit(EXIT_FAILURE);
    }

    if (p.vector[posicion] < cantidad)
    {
        cantidad = p.vector[posicion];
    }

    p.vector[posicion] -= cantidad;

    if (pthread_mutex_unlock(p.semaforo) != 0)
    {
        printf("Error en pthread_mutex_lock()\n");
        exit(EXIT_FAILURE);
    }

    printf("<-- Cliente %lu compró %d unidades de la camiseta %d\n", pthread_self(), cantidad, posicion);

    pthread_exit((void *)NULL);
}

void *simularSuministro(void *parametro)
{
    struct param p = *(struct param *)parametro;
    int posicion = rand() % (p.M);
    int cantidad = (rand() % 10) + 1;

    if (pthread_mutex_lock(p.semaforo) != 0)
    {
        printf("Error en pthread_mutex_lock()\n");
        exit(EXIT_FAILURE);
    }

    p.vector[posicion] += cantidad;

    if (pthread_mutex_unlock(p.semaforo) != 0)
    {
        printf("Error en pthread_mutex_lock()\n");
        exit(EXIT_FAILURE);
    }

    printf("--> Proveedor %lu suministró %d unidades de la camiseta %d\n", pthread_self(), cantidad, posicion);

    pthread_exit((void *)NULL);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Error, debes indicar N y M obligatoriamente.\n");
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int *vector = (int *)malloc(sizeof(int) * M);
    srand(time(NULL));
    pthread_t hilosClientes[N], hilosProveedores[M];
    pthread_mutex_t semaforo;
    struct param p;

    p.semaforo = &semaforo;
    p.vector = vector;
    p.M = M;

    if (vector == NULL)
    {
        printf("Error, ha fallado la reserva de memoria del vector.\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < M; i++)
    {
        vector[i] = (rand() % 100) + 1;
    }

    printf("Stock de %d tipos de camisetas antes de abrir la tienda:\n", M);
    for (size_t i = 0; i < M; i++)
    {
        printf("%d  ", vector[i]);
    }
    printf("\n");

    if (pthread_mutex_init(&semaforo, NULL) != 0)
    {
        printf("Error en pthread_mutex_init()\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < N; i++)
    {
        if (pthread_create(&hilosClientes[i], NULL, (void *)simularCompra, (void *)&p) != 0)
        {
            printf("Error en pthread_create().\n");
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i = 0; i < M; i++)
    {
        if (pthread_create(&hilosProveedores[i], NULL, (void *)simularSuministro, (void *)&p) != 0)
        {
            printf("Error en pthread_create().\n");
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i = 0; i < N; i++)
    {
        if (pthread_join(hilosClientes[i], (void **)NULL) != 0)
        {
            printf("Error en pthread_join().\n");
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i = 0; i < M; i++)
    {
        if (pthread_join(hilosProveedores[i], (void **)NULL) != 0)
        {
            printf("Error en pthread_join().\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("\nStock de %d tipos de camisetas al cerrar la tienda:\n", M);
    for (size_t i = 0; i < M; i++)
    {
        printf("%d  ", vector[i]);
    }
    printf("\n");


    if (pthread_mutex_destroy(&semaforo) != 0)
    {
        printf("Error en sem_destroy()\n");
        exit(EXIT_FAILURE);
    }

    free(vector);
    exit(EXIT_SUCCESS);
}