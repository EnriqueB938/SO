/*
Dados dos vectores de tamaño N y una cantidad de hilos M, donde siempre
M<N. Realice el producto escalar en paralelo de esos dos vectores. Para ello cree M hebras o
hilos que se repartan de manera equitativa los elementos de cada vector.
Cada hilo debe imprimir los elementos que le han tocado y el resultado final de la
multiplicación, la cual además envía al padre. El padre debe esperar por la terminación de cada
hijo y mostrar el valor escalar resultante.
Recuerde que el producto escalar de dos vectores es un número p = v1[0]*v2[0] +
v1[1]*v2[1] + v1[2]*v2[2] + … + v1[N-1]*v2[N-1].
Además, el proceso principal debe asegurarse el tratamiento de la señal SIGINT de
forma que si se recibe esta señal se debe mostrar por pantalla “Se ha recibido la señal SIGINT
pero se ha ignorado ya que aún quedan hilos en ejecución.”. 

REPARTIR:
int *vector;
int *vector2;
vector=(int*)malloc(sizeof(int)*N);
vector2=(int*)malloc(sizeof(int)*N);

rellenamos con numero random

struct param p[M];

for(int i=0;i<M;i++)
{
    p[i].v1=vector1;
    p[i].v2=vector2;
    p[i].inicio= i*(N/M);
    if(i==N-1)
    {
        p[i].fin=N;
    }else{
        p[i].fin=p[i].inicio+ (N/M);
    }
}
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

struct param
{
    int *v1;
    int *v2;
    int inicio;
    int fin;
};

void funcionManejadora(int signal)
{
    printf("He recibido la señal SIGINT(%d) y la ignoro...\n", signal);
}

void *funcionHilo(void *parametro)
{
    struct param p = *(struct param *)parametro;

    printf("Hilo %lu.... Hago el producto desde la poscion %d hasta %d.\n", pthread_self(), p.inicio, p.fin);

    int resultado = 0;
    for (size_t i = p.inicio; i < p.fin; i++)
    {
        resultado += (p.v1[i] * p.v2[i]);
        sleep(5);
    }

    int *retorno = (int *)malloc(sizeof(int));
    (*retorno) = resultado;
    pthread_exit((void *)retorno);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Error en línea de argumentos, debes introducir el tamaño del vector y el número de hilos.\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, &funcionManejadora);

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    srand(time(NULL));

    if (M >= N)
    {
        printf("M debe ser menor que N.\n");
        exit(EXIT_FAILURE);
    }

    int *vector1 = (int *)malloc(sizeof(int) * N);
    int *vector2 = (int *)malloc(sizeof(int) * N);

    for (size_t i = 0; i < N; i++)
    {
        vector1[i] = rand() % 100;
        vector2[i] = rand() % 100;
    }

    printf("Los vectores son:\n");
    for (size_t i = 0; i < N; i++)
    {
        printf("%d  ", vector1[i]);
    }
    printf("\n");
    for (size_t i = 0; i < N; i++)
    {
        printf("%d  ", vector2[i]);
    }
    printf("\n");

    pthread_t hilos[M];
    struct param p[M];

    for (size_t i = 0; i < M; i++)
    {
        p[i].v1 = vector1;
        p[i].v2 = vector2;
        p[i].inicio = i * (N / M);
        if (i == M - 1)
        {
            p[i].fin = N;
        }
        else
        {
            p[i].fin = p[i].inicio + (N / M);
        }

        if (pthread_create(&hilos[i], NULL, (void *)funcionHilo, (void *)&p[i]) != 0)
        {
            printf("Error en pthread_create()\n");
            exit(EXIT_FAILURE);
        }
    }

    int *valorRecogido;
    int suma = 0;
    for (size_t i = 0; i < M; i++)
    {
        if (pthread_join(hilos[i], (void **)&valorRecogido) != 0)
        {
            printf("Error en pthread_join()\n");
            exit(EXIT_FAILURE);
        }
        suma += (*valorRecogido);
        free(valorRecogido);
    }

    printf("El producto vectorial calculado es %d.\n", suma);

    free(vector1);
    free(vector2);
    exit(EXIT_SUCCESS);
}