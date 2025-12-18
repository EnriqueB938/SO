#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

void *funcionHilo(void *parametro)
{
    int n = rand() % 100;
    printf("Soy el hilo %lu y he generado el número random %d.\n", pthread_self(), n);

    int *retorno = (int *)malloc(sizeof(int));
    (*retorno) = n;
    pthread_exit((void *)retorno);
}

int main()
{
    srand(time(NULL));
    pthread_t idHilo;

    if (pthread_create(&idHilo, NULL, (void *)funcionHilo, (void *)NULL) != 0)
    {
        printf("Error en pthread_create().\n");
        exit(EXIT_FAILURE);
    }

    int *valorRecogido;
    if (pthread_join(idHilo, (void **)&valorRecogido) != 0)
    {
        printf("Error en pthread_join().\n");
        exit(EXIT_FAILURE);
    }

    printf("[PADRE]: El hilo %lu ha finalizado y ha devuelto el valor %d.\n", idHilo, (*valorRecogido));
    free(valorRecogido);

    exit(EXIT_SUCCESS);
}