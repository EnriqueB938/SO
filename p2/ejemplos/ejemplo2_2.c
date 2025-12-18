/*
pthread_create:
1. El & del id del hilo(direccion de memoria(imprimida en formato decimal)).
2. Siempre se podnra un NULL. Un strcut que tienes que pasarle un & ero con el NULL le das la configuracion por defecto.
3. El nombre de la funcion que se tiene que ejecutar (hay que hacer casting(forzar a que algo sea de un tipo concreto en este caso (void *)))
4.Casting--> Direccion de memoria del argumento de la funcion del hilo, pondremos NULL cuando el hilo no reciba nada.

pthread_exit:
1.Siempre se devuelve una direccion de memoria, aunque no devuelva nada, lo ponemos igual usando el NULL.

pthread_self:
1.Guarda la direccion de memoria del hilo.

pthread_join:
1. El id del hilo que tiene que esperar, ya que no necesita la direccion de memoria, no se le pone el &.
2. Direccion de memoria del puntero que recoge el valor devuelto por el hilo, no devuelve nada por eso pone NULL.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *funcionHilo(void *parametro)
{
    printf("Soy el hilo %lu.\n", pthread_self());
    pthread_exit((void *)NULL);
}

int main()
{
    pthread_t idHilo;

    if (pthread_create(&idHilo, NULL, (void *)funcionHilo, (void *)NULL) != 0)
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