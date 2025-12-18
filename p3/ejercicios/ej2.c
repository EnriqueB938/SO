/*
Implemente un programa nombrado "ejercicio2.c que cree dos hilos (hebras) en paralelo y que cumpla con los siguientes requisitos:
* Se tiene una variable compartida entre ambos hilos que está inicializada a 1.
* Se recogen por linea de argumentos dos valores enteros. Al primer hilo se le pasará en su creación el primer entero, 
y al segundo hilo el segundo entero.
* Al primer hilo se le asignará una función h1() que realizará lo siguiente:
* Suma a la variable compartida el entero que recibe en su creación.
* Muestra por pantalla el valor actual de la variable compartida.
* Si tras la suma el valor de la variable compartida es par, el hilo devuelve un 0, sino un 1.
* Al segundo hilo se le asignará una función h2() que realizará lo siguiente:
* Multiplica la variable compartida por el entero que recibe en su creación.
* Muestra por pantalla el valor actual de la variable compartida.
* Si tras la multiplicación el valor de la variable compartida es par, el hilo devuelve un 0, sino un 1.
* La variable compartida debe protegerse de posibles problemas de concurrencia usando semáforos.
* La hebra principal o main() recogerá el valor devuelto por cada una de las dos hebras, 
mostrando por pantalla el identificador de hebra y dicho valor devuelto.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

int compartida = 1;

struct param 
{
sem_t *semaforo;
int n1;
int n2;
};

void *funcionHilo1(void *parametro)
{
    struct param p = *(struct param *)parametro;
    
    if (sem_wait(p.semaforo) != 0)
    {
        printf("Error en sem_wait()\n");
        exit(EXIT_FAILURE);
    }

    compartida = compartida + p.n1;
    printf("[HILO 1] Variable compartida tras sumar el valor %d: %d\n", p.n1, compartida);
    
    if(compartida % 2 != 0)
    {
        pthread_exit((void*)1);
    }
    pthread_exit((void*)0);

    if (sem_post(p.semaforo) != 0)
    {
        printf("Error en pthread_mutex_unlock()\n");
        exit(EXIT_FAILURE);
    }


}

void *funcionHilo2(void *parametro)
{
    struct param p = *(struct param *)parametro;
    
    if (sem_wait(p.semaforo) != 0)
    {
        printf("Error en sem_wait()\n");
        exit(EXIT_FAILURE);
    }

    compartida = compartida * p.n2;

    printf("[HILO 2] La variable compartida tras sumar el valor %d: %d\n", p.n2 , compartida);

    if(compartida % 2 != 0)
    {
        pthread_exit((void*)1);
    }
    pthread_exit((void*)0);

    if (sem_post(p.semaforo) != 0)
    {
        printf("Error en pthread_mutex_unlock()\n");
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Error....\n");
        exit(EXIT_FAILURE);
    }

    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);
    struct param p;
    pthread_t hilo1, hilo2;
    sem_t semaforo;

    p.semaforo = &semaforo;
    p.n1 = num1;
    p.n2 = num2;

    if (sem_init(&semaforo, 0, 1) != 0)
    {
        printf("Error en pthread_mutex_init(semaforo)\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&hilo1, NULL, (void *)funcionHilo1, (void *)&p) != 0)
    {
        printf("Error en pthread_create().\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&hilo2, NULL, (void *)funcionHilo2, (void *)&p) != 0)
    {
        printf("Error en pthread_create().\n");
        exit(EXIT_FAILURE);
    }
    
    int *valorRecogido1, *valorRecogido2;

    if (pthread_join(hilo1, (void **)&valorRecogido1) != 0)
    {
        printf("Error en pthread_join().\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(hilo2, (void **)&valorRecogido2) != 0)
    {
        printf("Error en pthread_join().\n");
        exit(EXIT_FAILURE);
    }

    if (sem_destroy(&semaforo) != 0)
    {
        printf("Error en pthread_mutex_destroy(semaforo)\n");
        exit(EXIT_FAILURE);
    }

    printf("El HILO 1 (%lu) devolvió: %d\n ", hilo1, *valorRecogido1);
    printf("El HILO 2 (%lu) devolvió: %d\n ", hilo1, *valorRecogido2);

    free(valorRecogido1);
    free(valorRecogido2);

    exit(EXIT_SUCCESS);
}