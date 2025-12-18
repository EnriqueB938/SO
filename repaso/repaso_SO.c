#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int status;
pid_t pid_hijo;

struct param
{
    sem_t *semaforo; //Ambas forma de declarar un semaforo en la estructura
    pthread_mutex_t *semaforo; //forma 2 (depende de la biblioteca y de lo que queremos hacer)
    //Si queremos utilizar variables generales usamos el *
};

void espera()
{
    while ((pid_hijo = waitpid(-1, &status, 0)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("Proceso padre %d, hijo con PID %ld finalizado, status = %d\n", getpid(), (long int)pid_hijo, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status)) // Para seniales como las de finalizar o matar
        {
            printf("Proceso padre %d, hijo con PID %ld finalizado al recibir la señal %d\n", getpid(), (long int)pid_hijo, WTERMSIG(status));
        }
    }
    if (pid_hijo == (pid_t)-1 && errno == ECHILD) // Entra cuando vuelve al while y no hay más hijos que esperar
    {
        printf("Proceso padre %d, no hay mas hijos que esperar. Valor de errno = %d, definido como: %s\n", getpid(), errno, strerror(errno));
    }
    else
    {
        printf("Error en la invocacion de wait o waitpid. Valor de errno = %d, definido como: %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}


void funcionHilo(void *parametro)
{
    struct param p = *(struct param *)parametro;
    int i;
    for (i = 0; i < 5; i++)
    {
        if (sem_wait(p.semaforo) != 0)
        {
            printf("Error en sem_destroy()\n");
            exit(EXIT_FAILURE);
        }
        printf("B");
        if (sem_post(p.semaforo) != 0)
        {
            printf("Error en sem_destroy()\n");
            exit(EXIT_FAILURE);
        }
    }

    float *retorno = (float *)malloc(sizeof(float));
    (*retorno) = resultado;
    pthread_exit((void *)retorno);
    
    pthread_exit((void *)NULL);
}



void funcionManejadora(int signal)
{
    printf("Soy el proceso %ld y he recibido la señal %d...\n",(long int)getpid(), signal);
}



//MAIN PROCESOS E HIJOS
int main(int argc, char **argv)
{

    if(argc != 2)
    {
        printf("Error...\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    //Crear hijos

    for (int i = 0; i < 3; i++)
    {
        pid_t pid = fork();
        switch (pid)
        {
        case -1:
            printf("Error en el fork().\n");
            exit(EXIT_FAILURE);
        case 0:
            signal(SIGUSR1, &funcionManejadora);
            printf("[HIJOS]: soy el proceso con pid %ld y el pid de mi padre es %ld\n", (long int)getpid(), (long int)getppid());
            espera();
            exit(EXIT_SUCCESS);
        }
    }

}



//MAIN HILOS Y SEMAFOROS
int main(int argc, char **argv)
{

    if(argc != 2)
    {
        printf("Error en la linea de argumentos\n");
        exit(EXIT_FAILURE);
    }

    pthread_t hilo1, hilo2;
    srand(time(NULL));
    int N = atoi(argv[1]);
    int M = atoi(argv[2]);

    pthread_mutex_t semaforo; //Una forma de declarar un semaforo
    sem_t semaforo; //Otra forma (esta la usaremos cuando tengamos que utlizar la biblioteca semaphore.h (wait y post))

    if(pthread_mutex_init(&semaforo, NULL) != 0)
    {
        printf("Error en el init\n");
        exit(EXIT_FAILURE);
    }

    if(sem_init(&Semaforo, 0, 1) != 0)
    {
        printf("Error en el init\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0;i<N;i++)
    {
        if(pthread_create(&hilos, NULL, (void *)funcionHilo, (void **)NULL /*Aqui podemos poner un valor (entero, flotante, etc..) */) != 0)
        {
            printf("Error create\n");
            exit(EXIT_FAILURE);
        }
    }

    void *valorRecogido;

    if(pthread_join(hilos, (void **)&valorRecogido) != 0)
    {
        printf("Error en el join\n");
        exit(EXIT_FAILURE);
    }

    free(valorRecogido);

    if(pthread_mutex_destroy(&semaforo) != 0)   
    {
        printf("Error en el destroy\n");
        exit(EXIT_FAILURE);
    }

    if(sem_destroy(&semaforo) != 0)
    {
        printf("Error en el destroy\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}



