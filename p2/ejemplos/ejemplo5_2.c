#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct param
{
    int n1;
    float n2;
};

void *funcionHilo(void *parametro)
{
    struct param p = *((struct param *)parametro);
    printf("Soy el hilo %lu y recibido el entero %d y el flotante %f.\n", pthread_self(), p.n1, p.n2);
    pthread_exit((void *)NULL);
}

int main()
{
    pthread_t idHilo;

    struct param p;
    printf("Introduce el entero: ");
    scanf("%d", &p.n1);
    printf("Introduce el flotante: ");
    scanf("%f", &p.n2);

    if (pthread_create(&idHilo, NULL, (void *)funcionHilo, (void *)&p) != 0)
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