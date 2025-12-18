#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct param
{
    long int *global;
    pthread_mutex_t *semaforo;
};

void *funcionHilo(void *parametro)
{
    struct param p = *(struct param *)parametro;
    for (size_t i = 0; i < 1000000; i++)
    {
        if (pthread_mutex_lock(p.semaforo) != 0)
        {
            printf("Error en pthread_mutex_lock()\n");
            exit(EXIT_FAILURE);
        }
        (*p.global)++;
        if (pthread_mutex_unlock(p.semaforo) != 0)
        {
            printf("Error en pthread_mutex_unlock()\n");
            exit(EXIT_FAILURE);
        }
    }

    pthread_exit((void *)NULL);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Error....\n");
        exit(EXIT_FAILURE);
    }

    long int global = 0;
    pthread_mutex_t semaforo;

    printf("[PADRE]: la variable global contiene: %ld\n", global);

    int N = atoi(argv[1]);

    pthread_t idHilos[N];
    struct param p;

    p.global = &global;
    p.semaforo = &semaforo;

    if (pthread_mutex_init(&semaforo, NULL) != 0)
    {
        printf("Error en pthread_mutex_init()\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < N; i++)
    {
        if (pthread_create(&idHilos[i], NULL, (void *)funcionHilo, (void *)&p) != 0)
        {
            printf("Error en pthread_create().\n");
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i = 0; i < N; i++)
    {
        if (pthread_join(idHilos[i], (void **)NULL) != 0)
        {
            printf("Error en pthread_join().\n");
            exit(EXIT_FAILURE);
        }

        // printf("[PADRE]: El hilo %lu ha finalizado.\n", idHilos[i]);
    }

    if (pthread_mutex_destroy(&semaforo) != 0)
    {
        printf("Error en pthread_mutex_destroy()\n");
        exit(EXIT_FAILURE);
    }

    printf("[PADRE]: la variable global contiene: %ld\n", global);

    exit(EXIT_SUCCESS);
}


/*
Repetir el ejemplo 1.
Pero esta prohibido el uso de variables globales.
*/