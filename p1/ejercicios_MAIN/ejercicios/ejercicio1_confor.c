/*
        a.out
         /  \
        H1  H2
         |  |
        N1  N2
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

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

int main()
{
    printf("[PADRE]: soy el proceso con pid %ld y el pid de mi padre es %ld\n", (long int)getpid(), (long int)getppid());

    for (size_t i = 0; i < 2; i++)
    {
        pid_t pid = fork();
        switch (pid)
        {
        case -1:
            printf("Error en el fork().\n");
            exit(EXIT_FAILURE);
        case 0:
            printf("[HIJO]: soy el proceso con pid %ld y el pid de mi padre es %ld\n", (long int)getpid(), (long int)getppid());
            pid_t pid = fork();
            switch (pid)
            {
            case -1:
                printf("Error en el fork().\n");
                exit(EXIT_FAILURE);
            case 0:
                printf("[HIJO]: soy el proceso con pid %ld y el pid de mi padre es %ld\n", (long int)getpid(), (long int)getppid());
                exit(EXIT_SUCCESS);
            }
            espera();
            exit(EXIT_SUCCESS);
        }
    }

    espera();
    exit(EXIT_SUCCESS);
}