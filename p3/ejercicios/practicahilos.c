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
    struct param p = *(struct param*)parametro;
    int posicion = rand() % (p.M);
    int cantidad = rand() % 10 + 1;

    if(pthread_mutex_lock(p.semaforo) != 0)
    {
        printf("Error en el lock(). \n");
        exit(EXIT_FAILURE);
    }

    if(p.vector[posicion] < cantidad)
    {
        cantidad = p.vector[posicion];
    }

    p.vector[posicion] -= cantidad;


    if(pthread_mutex_unlock(p.semaforo) != 0)
    {
        printf("Error en el unlock(). \n");
        exit(EXIT_FAILURE);
    }

    printf("\n El cliente %lu compro %d unidades de la camiseta %d \n", pthread_self(), cantidad, posicion);

    pthread_exit((void*)NULL);
}

void *simularSuministro(void *parametro)
{
    struct param p = *(struct param*)parametro;
    int posicion = rand() % (p.M);
    int cantidad = rand() % 10 + 1;

    if(pthread_mutex_lock(p.semaforo) != 0)
    {
        printf("Error en el lock(). \n");
        exit(EXIT_FAILURE);
    }

    p.vector[posicion] += cantidad;

    if(pthread_mutex_unlock(p.semaforo) != 0)
    {
        printf("Error en el unlock(). \n");
        exit(EXIT_FAILURE);
    }

    printf("\n El proveedor %lu suminsitro %d unidades de camisetas %d. \n", pthread_self(), cantidad, posicion);

    pthread_exit((void*)NULL);
}

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("Error en la linea de argumentos\n");
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int *vector = (int *)malloc(sizeof(int) * M);
    pthread_t hilosclientes[N], hilosproveedores[M];
    pthread_mutex_t semaforo;
    struct param p;

    p.semaforo = &semaforo;
    p.vector = vector;
    p.M = M;

    if(vector == NULL)
    {
        printf("Error a la hora de entrar al vector ya que esta vacio o lleno.\n");
        exit(EXIT_FAILURE);
    }

    for(size_t i = 0; i<M; i++)
    {
        vector[i] = (rand() % 100) + 1;
    }

    printf("Stock de %d tipos de camisetas antes de abrrir la tienda. \n", M);

    for(size_t i = 0; i<M; i++)
    {
        printf("%d ", vector[i]);
    }

    if(pthread_mutex_init(&semaforo, NULL) != 0)
    {
        printf("Error al inicilaizar el semaforo. \n");
        exit(EXIT_FAILURE);
    }

    for(size_t i = 0; i<N; i++)
    {
        if(pthread_create(&hilosclientes[i], NULL, (void*)simularCompra, (void*)&p) != 0)
        {
            printf("Error create. \n");
            exit(EXIT_FAILURE);
        }
    }

    for(size_t i = 0; i<M; i++)
    {
        if(pthread_create(&hilosproveedores[i], NULL, (void*)simularSuministro, (void*)&p) != 0)
        {
            printf("Error create. \n");
            exit(EXIT_FAILURE);
        }
    }

    for(size_t i = 0; i<N; i++)
    {
        if(pthread_join(hilosclientes[i], (void**)NULL) != 0)
        {
            printf("Error en el join(). \n");
            exit(EXIT_FAILURE);
        }
    }

    for(size_t i = 0; i<M; i++)
    {
        if(pthread_join(hilosproveedores[i], (void**)NULL) != 0)
        {
            printf("Error en el join(). \n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Stock de %d de tipo de camisetas dsp de cerrar la tienda: \n", M);

    for(size_t i = 0; i<M; i++)
    {
        printf("%d ", vector[i]);
    }

    printf("\n");

    if(pthread_mutex_destroy(&semaforo) != 0)
    {
        printf("Error en el destroy(). \n");
        exit(EXIT_FAILURE);
    }

    free(vector);
    exit(EXIT_SUCCESS);


}