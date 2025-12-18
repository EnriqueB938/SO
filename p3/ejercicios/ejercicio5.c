#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

struct param
{
    sem_t *semA, *semB;
};

void escribirA(void *parametro)
{
    struct param p = *(struct param *)parametro;
    int i;
    for (i = 0; i < 5; i++)
    {
        if (sem_wait(p.semA) != 0)
        {
            printf("Error en sem_destroy()\n");
            exit(EXIT_FAILURE);
        }
        usleep(500);
        printf("A");
        fflush(NULL);
        
        if (sem_post(p.semB) != 0)
        {
            printf("Error en sem_destroy()\n");
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit((void *)NULL);
}

void escribirB(void *parametro)
{
    struct param p = *(struct param *)parametro;
    int i;
    for (i = 0; i < 5; i++)
    {
        if (sem_wait(p.semB) != 0)
        {
            printf("Error en sem_destroy()\n");
            exit(EXIT_FAILURE);
        }
        usleep(500);
        printf("B");
        fflush(NULL);
        if (sem_post(p.semA) != 0)
        {
            printf("Error en sem_destroy()\n");
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit((void *)NULL);
}

int main()
{
    pthread_t hilo1, hilo2;
    struct param p;
    sem_t semA, semB;

    p.semA = &semA;
    p.semB = &semB;

    if (sem_init(&semA, 0, 1) != 0)
    {
        printf("Error en sem_init()\n");
        exit(EXIT_FAILURE);
    }

    if (sem_init(&semB, 0, 0) != 0)
    {
        printf("Error en sem_init()\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&hilo1, NULL, (void *)escribirA, (void *)&p) != 0)
    {
        printf("Error en pthread_create(hilo1).\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&hilo2, NULL, (void *)escribirB, (void *)&p) != 0)
    {
        printf("Error en pthread_create(hilo2).\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(hilo1, (void **)NULL) != 0)
    {
        printf("Error en pthread_join(hilo1).\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(hilo2, (void **)NULL) != 0)
    {
        printf("Error en pthread_join(hilo2).\n");
        exit(EXIT_FAILURE);
    }

    printf("\n");

    if (sem_destroy(&semA) != 0)
    {
        printf("Error en sem_destroy()\n");
        exit(EXIT_FAILURE);
    }

    if (sem_destroy(&semB) != 0)
    {
        printf("Error en sem_destroy()\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}