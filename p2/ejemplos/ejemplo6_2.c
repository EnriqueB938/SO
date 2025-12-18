#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

void *funcionHilo(void *parametro)
{
    int numero = *((int *)parametro);
    printf("Soy el hilo %lu y he recibido el número %d.\n", pthread_self(), numero);
    pthread_exit((void *)NULL);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Error....\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    int N = atoi(argv[1]);

    pthread_t idHilos[N];

    int numero[N];

    for (size_t i = 0; i < N; i++)
    {
        numero[i] = rand() % 100;
        if (pthread_create(&idHilos[i], NULL, (void *)funcionHilo, (void *)&numero[i]) != 0)
        {
            printf("Error en pthread_create().\n");
            exit(EXIT_FAILURE);
        }
        printf("MAIN()--> Creado el hilo %lu con el número random %d\n", idHilos[i], numero[i]);
    }

    for (size_t i = 0; i < N; i++)
    {
        if (pthread_join(idHilos[i], (void **)NULL) != 0)
        {
            printf("Error en pthread_join().\n");
            exit(EXIT_FAILURE);
        }

        printf("[PADRE]: El hilo %lu ha finalizado.\n", idHilos[i]);
    }

    exit(EXIT_SUCCESS);
}