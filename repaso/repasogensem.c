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


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//#include <semaphore.h>

struct param
{
    pthread_mutex_t *semaforo; //Variable global
    int *vector;  //Varaible global
    int M;  //M es el valor de comerciantes y el del modelo de la camiseta por lo tanto es importante dejarlo en el struct
};

void *simularCompra(void *parametro)
{
    struct param p = *(struct param *)parametro;
    int posicion = rand() % (p.M);
    int cantidad = (rand() % 10) + 1;

    if(pthread_mutex_lock(p.semaforo) != 0)
    {
        printf("Error en el pthread_mutex_lock(compra). \n");
        exit(EXIT_FAILURE);
    }

    if(p.vector[posicion] < cantidad)
    {
        cantidad = p.vector[posicion];
    }

    p.vector[posicion] -= cantidad;

    if(pthread_mutex_unlock(p.semaforo) != 0)
    {
        printf("Error en el pthread_mutex_unlock(comprar). \n");
        exit(EXIT_FAILURE);
    }

    printf("\n El Cliente %lu compró %d unidades de la camiseta %d \n", pthread_self(), cantidad, posicion);

    pthread_exit((void*)NULL);

}

void *simularSuministro(void *parametro)
{
    struct param p = *(struct param *)parametro;
    int posicion = rand() % p.M;
    int cantidad = (rand() % 10) + 1;

    if(pthread_mutex_lock(p.semaforo) != 0)
    {
        printf("Error en el pthread-mutex_lock(suministro). \n");
        exit(EXIT_FAILURE);
    }

    p.vector[posicion] += cantidad;

    if(pthread_mutex_unlock(p.semaforo) != 0)
    {
        printf("Error en el pthread_mutex_unlock(suministro). \n");
        exit(EXIT_FAILURE);
    }

    printf("El proveedor %lu suministro %d unidades de la camiseta %d. \n", pthread_self(), posicion, cantidad);

    pthread_exit((void*)NULL);
}


int main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("Error en la linea de argumnetos debes introducir mas argumentos.\n");
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int *vector = (int *)malloc(sizeof(int) * M);
    srand(time(NULL));
    pthread_t hilos_clientes[N], hilos_proveedores[M];
    pthread_mutex_t semaforo;
    struct param p;

    p.semaforo = &semaforo;
    p.vector = vector;
    p.M = M;

    if(vector == NULL)
    {
        printf("Error ha fallado a la hora de reservar memoria.\n");
        exit(EXIT_FAILURE);
    }

    for(size_t i = 0; i<M; i++)
    {
        vector[i] = (rand() % 100) + 1;
    }

    printf("Stock de %d tipos de camisetas antes de abrir la tienda. \n", M);

    for(size_t i = 0; i<M; i++)
    {
        printf("%d ", vector[i]);
    }

    if(pthread_mutex_init(&semaforo, NULL) != 0)
    {
        printf("Error en el pthread_mutex_init(). \n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < N; i++)
    {
        if(pthread_create(&hilos_clientes[i], NULL, (void*)simularCompra, (void *)&p) != 0)
        {
            printf("Error en el pthread_create(clientes). \n");
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i = 0; i < M; i++)
    {
        if(pthread_create(&hilos_proveedores[i], NULL, (void*)simularSuministro, (void *)&p) != 0)
        {
            printf("Error en el pthread_create(proveedores). \n");
            exit(EXIT_FAILURE);
        }
    }

    for(size_t i = 0;i< N; i++)
    {
        if(pthread_join(hilos_clientes[i], (void **)NULL) != 0)
        {
            printf("Error en el pthread_join(clientes). \n");
            exit(EXIT_FAILURE);
        }
    }

    for(size_t i = 0;i< M; i++)
    {
        if(pthread_join(hilos_proveedores[i], (void **)NULL) != 0)
        {
            printf("Error en el pthread_join(proveedores). \n");
            exit(EXIT_FAILURE);
        }
    }

    printf("\n Stock de %d tipos de camisetas al cerrar la tienda: \n", M);

    for(size_t i = 0; i< M; i++)
    {
        printf("%d ", vector[i]);
    }

    printf("\n");

    if(pthread_mutex_destroy(&semaforo) != 0)
    {
        printf("Error en el pthread_mutex_destroy()");
        exit(EXIT_FAILURE);
    }

    free(vector);
    exit(EXIT_SUCCESS);  //Lo mismo que hacer return 0

}