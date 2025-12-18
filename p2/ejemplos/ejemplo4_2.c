#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *funcionHilo(void *parametro)
{
    int numero = *(int *)parametro;
    printf("Soy el hilo %lu y he recibido el número %d.\n", pthread_self(), numero);
    pthread_exit((void *)NULL);
}

int main()
{
    pthread_t idHilo;

    srand(time(NULL));

    int numero = rand() % 100;
    printf("Padre: generado el número %d.\n", numero);

    if (pthread_create(&idHilo, NULL, (void *)funcionHilo, (void *)&numero) != 0)
    {
        printf("Error en pthread_create().\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(idHilo, (void **)NULL) != 0)
    {
        printf("Error en pthread_join().\n");
        exit(EXIT_FAILURE);
    }

    printf("[PADRE]: El hilo %lu ha finalizado.\n", idHilo);

    exit(EXIT_SUCCESS);
}