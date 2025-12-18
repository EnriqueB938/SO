/*
Crear un programa en el que el proceso principal main, debe de crear 2 hilos,
1.Recibe un entero por parte del main.
2.Recibe un flotante por parte del main.
Tanto el entero como el flotante de se introduciran por teclado, los hilos deben mostrar su identificador y el valor recibido,
además el primer hilo debe retornar al main el entero recibido y sumarle 1.5 y el hilo 2 debe retornar al hilo el flotante por 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *funcionHilo1(void *parametro)
{
    int numero = *((int *)parametro);
    printf("Soy el hilo %lu y he recibido el entero %d.\n", pthread_self(), numero);
    float resultado = numero + 1.5;

    float *retorno = (float *)malloc(sizeof(float));
    (*retorno) = resultado;
    pthread_exit((void *)retorno);
}

void *funcionHilo2(void *parametro)
{
    float numero = *((float *)parametro);
    printf("Soy el hilo %lu y he recibido el flotante %f.\n", pthread_self(), numero);
    float resultado = numero * 2;

    float *retorno = (float *)malloc(sizeof(float));
    (*retorno) = resultado;
    pthread_exit((void *)retorno);
}

int main()
{
    pthread_t idHilo1, idHilo2;
    int entero;
    float flotante;

    printf("Introduce el entero: ");
    scanf("%d", &entero);
    printf("Introduce el flotante: ");
    scanf("%f", &flotante);

    if (pthread_create(&idHilo1, NULL, (void *)funcionHilo1, (void *)&entero) != 0)
    {
        printf("Error en pthread_create().\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&idHilo2, NULL, (void *)funcionHilo2, (void *)&flotante) != 0)
    {
        printf("Error en pthread_create().\n");
        exit(EXIT_FAILURE);
    }

    float *valorRecogido;

    if (pthread_join(idHilo1, (void **)&valorRecogido) != 0)
    {
        printf("Error en pthread_join().\n");
        exit(EXIT_FAILURE);
    }
    printf("[PADRE]: El hilo %lu ha finalizado y ha devuelto un %f\n", idHilo1, (*valorRecogido));
    free(valorRecogido);

    if (pthread_join(idHilo2, (void **)&valorRecogido) != 0)
    {
        printf("Error en pthread_join().\n");
        exit(EXIT_FAILURE);
    }
    printf("[PADRE]: El hilo %lu ha finalizado y ha devuelto un %f\n", idHilo2, (*valorRecogido));
    free(valorRecogido);

    exit(EXIT_SUCCESS);
}