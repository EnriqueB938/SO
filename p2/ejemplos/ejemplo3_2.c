#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *funcionHilo(void *parametro)
{
    printf("Soy el hilo %lu.\n", pthread_self());
    pthread_exit((void *)NULL);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Error....\n");
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);

    pthread_t idHilos[N];

    for (size_t i = 0; i < N; i++)
    {
        if (pthread_create(&idHilos[i], NULL, (void *)funcionHilo, (void *)NULL) != 0)
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

        printf("[PADRE]: El hilo %lu ha finalizado.\n", idHilos[i]);
    }

    exit(EXIT_SUCCESS);
}