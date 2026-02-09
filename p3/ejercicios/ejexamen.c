#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

struct param
{
    pthread_mutex_t *semaforo;
    int *compartida;
    int valor;
};

void *sumador(void *parametro)
{
    struct param p = *(struct param*)parametro;

    printf("Hilo sumador %lu, valor recibido = %d \n", pthread_self(), p.valor);

    if(pthread_mutex_lock(p.semaforo) != 0)
    {
        printf("Error en el lock(). \n");
        exit(EXIT_FAILURE);
    }

    for(size_t i = 0; i<p.valor; i++)
    {
        (*p.compartida)++;
    }

    if(pthread_mutex_unlock(p.semaforo) != 0)
    {
        printf("Error en el unlock(). \n");
        exit(EXIT_FAILURE);
    }

    int *resultado = (int*)malloc(sizeof(int));
    if(*p.compartida % 2 == 0)
    {
        *resultado = 1;
        printf("MAIN(), el hilo %lu ha devuelto que la variable compartida es par. \n", pthread_self());
    }
    else
    {
        *resultado = 0;
        printf("Main(), el hilo %lu ha devuelto que la variable compartida es impar. \n", pthread_self());
    }

    pthread_exit((void*)NULL);
}

void *restador(void * parametro)
{
    struct param p = *(struct param*)parametro;

    printf("Hilo restador %lu, valor recibido = %d \n", pthread_self(), p.valor);

    if(pthread_mutex_lock(p.semaforo) != 0)
    {
        printf("Error en el lock(). \n");
        exit(EXIT_FAILURE);
    }

    for(size_t i = 0; i<p.valor; i++)
    {
        (*p.compartida)--;
    }

    if(pthread_mutex_unlock(p.semaforo) != 0)
    {
        printf("Error en el unlock(). \n");
        exit(EXIT_FAILURE);
    }

    int *resultado = (int*)malloc(sizeof(int));
    if(*p.compartida % 2 == 0)
    {
        *resultado = 1;
        printf("MAIN(), el hilo %lu ha devuelto que la variable compartida es par. \n", pthread_self());
    }
    else
    {
        *resultado = 0;
        printf("Main(), el hilo %lu ha devuelto que la variable compartida es impar. \n", pthread_self());
    }

    pthread_exit((void*)NULL);
}

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("Error en la linea de argumentos. \n");
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    srand(time(NULL));
    int compartida = 0;
    pthread_mutex_t semaforo;
    pthread_t sumadores[n], restadores[m];
    struct param p;


    if(pthread_mutex_init(&semaforo, NULL) != 0)
    {
        printf("Error en el init(). \n");
        exit(EXIT_FAILURE);
    }

    for(size_t i = 0; i<n; i++)
    {
        p.semaforo = &semaforo;
        p.compartida = &compartida;
        p.valor = rand() % 20 + 1;
        if(pthread_create(&sumadores[i], NULL, (void*)sumador, (void*)&p) != 0)
        {
            printf("Error en el create(). \n");
            exit(EXIT_FAILURE);
        }
    }

    for(size_t i = 0; i<m; i++)
    {
        p.semaforo = &semaforo;
        p.compartida = &compartida;
        p.valor = rand() % 20 + 1;
        if(pthread_create(&restadores[i], NULL, (void*)restador, (void*)&p) != 0)
        {
            printf("Error en el create(). \n");
            exit(EXIT_FAILURE);
        }
    }

    

    for(size_t i = 0; i<n; i++)
    {
        if(pthread_join(sumadores[i], (void**)NULL) != 0)
        {
            printf("Error en el join(). \n");
            exit(EXIT_FAILURE);
        }
    }

    for(size_t i = 0; i<m; i++)
    {
        if(pthread_join(restadores[i], (void**)NULL) != 0)
        {
            printf("Error en el join(). \n");
            exit(EXIT_FAILURE);
        } 
    }

    printf("MAIN(), valor de la variable compartida al finalizar = %d \n", compartida);

    if(pthread_mutex_destroy(&semaforo) != 0)
    {
        printf("Error en el destroy(). \n");
        exit(EXIT_FAILURE);
    }

    
    exit(EXIT_SUCCESS);

}