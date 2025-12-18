#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <time.h>

int status;
pid_t pid_hijo;

void espera()
{
    if(WEXITSTATUS(status)>=1 && WEXITSTATUS(status)<=10)
    {
        printf("Hijo %ld me ha devuelto el valor %d. \n", (long int)pid_hijo, WEXITSTATUS(status));
    }
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


void funcionManejadora(int signal)
{
    printf("Soy el proceso %ld y he recibido la señal %d...\n",(long int)getpid(), signal);
}


int main(int argc, char **argv)
{
    if(argc!=4)
    {
        printf("Error en la línea de argumentos\n");
        exit(EXIT_FAILURE);
    }

    printf("[PADRE]: soy el proceso con pid %ld y el pid de mi padre es %ld\n", (long int)getpid(), (long int)getppid());

    char ejecutable[100];

    pid_t pid = fork();

    switch (pid)
    {
    case -1:
        printf("Error en el fork().\n");
        exit(EXIT_FAILURE);
    case 0:
        printf("[HIJO 1]: soy el proceso con pid %ld y el pid de mi padre es %ld\n", (long int)getpid(), (long int)getppid());
        srand(time(NULL));
        int numero = (rand() % 10 + 1);
        printf("Proceso hijo %ld genera el número %d y lo retorna al padre....\n", (long int )getpid(), numero);
        exit(EXIT_SUCCESS);
    }

    pid = fork();

    switch (pid)
    {
    case -1:
        printf("Error en el fork().\n");
        exit(EXIT_FAILURE);
    case 0:
        printf("[HIJO 2]: soy el proceso con pid %ld y el pid de mi padre es %ld\n", (long int)getpid(), (long int)getppid());
        sprintf(ejecutable, "./%s", argv[2]);
        if(execvp(ejecutable, &argv[3-1])==-1)
        {
            printf("Error en el execvp.\n");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }

    pid = fork();

    switch (pid)
    {
    case -1:
        printf("Error en el fork().\n");
        exit(EXIT_FAILURE);
    case 0:
        signal(SIGUSR1, &funcionManejadora);
        printf("[HIJO 3]: soy el proceso con pid %ld y el pid de mi padre es %ld\n", (long int)getpid(), (long int)getppid());
        while(1)
        {
            pause();
        }
        exit(EXIT_SUCCESS);
    default:
        for(int i=0; i< atoi(argv[1]); i++)
        {
            sleep(1);
            kill(pid, SIGUSR1);
        }
        sleep(1);
        kill(pid, SIGKILL);
    }
    espera();
    exit(EXIT_FAILURE);

}