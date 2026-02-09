#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

void funcionManejadora(int sig) 
{
    printf("\nRecibida la señal SIGINT: se ha ignorado\n");
}

void espera()
{
    int status;
    pid_t pid_hijo;
    while ((pid_hijo = waitpid(-1, &status, 0)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("Proceso padre %d, hijo con PID %ld finalizado, status = %d\n", getpid(), (long int)pid_hijo, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status)) 
        {
            printf("Proceso padre %d, hijo con PID %ld finalizado al recibir la señal %d\n", getpid(), (long int)pid_hijo, WTERMSIG(status));
        }
    }
    if (pid_hijo == (pid_t)-1 && errno == ECHILD)
    {
        printf("Proceso padre %d, no hay mas hijos que esperar. Valor de errno = %d, definido como: %s\n", getpid(), errno, strerror(errno));
    }
    else
    {
        printf("Error en la invocacion de wait o waitpid. Valor de errno = %d, definido como: %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Error en la linea de argumentos. \n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        pid_t pid = fork();

        if (pid == -1) 
        {
            printf("Error en fork");
            exit(EXIT_FAILURE);
        } 
        else if (pid == 0) 
        {
            execl("./contarlineas", "contarlineas", argv[i], NULL);
            signal(SIGINT, &funcionManejadora);
            printf("Error en execl\n");
            kill(getppid(), SIGINT);
            exit(EXIT_FAILURE);
        }
    }

    espera();
    exit(EXIT_SUCCESS);
}