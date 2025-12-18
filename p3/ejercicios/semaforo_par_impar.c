#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

struct param
{
    int *par;
    int *impar;
    pthread_mutex_t *semaforoPar;
    pthread_mutex_t *semaforoImpar;
    int indiceCreacion;
};

void *funcionHilo(void *parametro)
{
    struct param p = *(struct param *)parametro;
    int suma = 0;

    for (size_t i = 0; i < 5; i++)
    {
        int n = rand() % 11;
        suma += n;

        if (p.indiceCreacion % 2 == 0)
        {
            if (pthread_mutex_lock(p.semaforoPar) != 0)
            {
                printf("Error en pthread_mutex_lock(semaforoPar)\n");
                exit(EXIT_FAILURE);
            }
            (*p.par) += n;
            if (pthread_mutex_unlock(p.semaforoPar) != 0)
            {
                printf("Error en pthread_mutex_unlock(semaforoPar)\n");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            if (pthread_mutex_lock(p.semaforoImpar) != 0)
            {
                printf("Error en pthread_mutex_lock(semaforoImpar)\n");
                exit(EXIT_FAILURE);
            }
            (*p.impar) += n;
            if (pthread_mutex_unlock(p.semaforoImpar) != 0)
            {
                printf("Error en pthread_mutex_unlock(semaforoImpar)\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    int *retorno = (int *)malloc(sizeof(int));
    (*retorno) = suma;
    pthread_exit((void *)retorno);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Error en línea de argumentos...\n");
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);
    struct param p[N];
    pthread_t hilos[N];
    int par = 0, impar = 0;
    pthread_mutex_t semaforoPar, semaforoImpar;
    srand(time(NULL));

    if (pthread_mutex_init(&semaforoPar, NULL) != 0)
    {
        printf("Error en pthread_mutex_init(semaforoPar)\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(&semaforoImpar, NULL) != 0)
    {
        printf("Error en pthread_mutex_init(semaforoImpar)\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < N; i++)
    {
        p[i].par = &par;
        p[i].impar = &impar;
        p[i].semaforoPar = &semaforoPar;
        p[i].semaforoImpar = &semaforoImpar;
        p[i].indiceCreacion = i + 1;
        if (pthread_create(&hilos[i], NULL, (void *)funcionHilo, (void *)&p[i]) != 0)
        {
            printf("Error en pthread_create().\n");
            exit(EXIT_FAILURE);
        }
    }

    int sumaPares = 0, sumaImpares = 0;
    int *valorRecogido;
    for (size_t i = 0; i < N; i++)
    {
        if (pthread_join(hilos[i], (void **)&valorRecogido) != 0)
        {
            printf("Error en pthread_join()\n");
            exit(EXIT_FAILURE);
        }

        printf("Main()... La hebra de orden de creación %d devolvió el valor de la suma total: %d\n", (int)i + 1, (*valorRecogido));
        
        if ((i + 1) % 2 == 0)
        {
            sumaPares += (*valorRecogido);
        }
        else
        {
            sumaImpares += (*valorRecogido);
        }

        free(valorRecogido);
    }

    if (pthread_mutex_destroy(&semaforoPar) != 0)
    {
        printf("Error en pthread_mutex_destroy(semaforoPar)\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_mutex_destroy(&semaforoImpar) != 0)
    {
        printf("Error en pthread_mutex_destroy(semaforoImpar)\n");
        exit(EXIT_FAILURE);
    }

    printf("\nMain()... Valor de la variable compartida impar: %d\n", impar);
    printf("Main()... Suma de los valores devueltos por las hebras de creación impares: %d\n", sumaImpares);

    printf("\nMain()... Valor de la variable compartida par: %d\n", par);
    printf("Main()... Suma de los valores devueltos por las hebras de creación pares: %d\n", sumaPares);

    exit(EXIT_SUCCESS);
}