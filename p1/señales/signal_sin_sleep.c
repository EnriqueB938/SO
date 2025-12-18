#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

pid_t pid, flag;
int status;

void espera()
{
    while ((flag = wait(&status)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("Proceso Padre, Hijo con PID %ld finalizado, status = %d\n", (long int)flag, WEXITSTATUS(status));
            if (WEXITSTATUS(status) != 0)
            {
                printf("Hijo %ld ha devuelto el valor %d.\n", (long int)flag, WEXITSTATUS(status));
            }
        }
        else if (WIFSIGNALED(status))
        { // Para seniales como las de finalizar o matar
            printf("Proceso Padre, Hijo con PID %ld finalizado al recibir la señal %d\n", (long int)flag, WTERMSIG(status));
        }
        else if (WIFSTOPPED(status))
        { // Para cuando se para un proceso. Al usar wait() en vez de waitpid() no nos sirve.
            printf("Proceso Padre, Hijo con PID %ld parado al recibir la señal %d\n", (long int)flag, WSTOPSIG(status));
        }
        else if (WIFCONTINUED(status))
        { // Para cuando se reanuda un proceso parado. Al usar wait() en vez de waitpid() no nos sirve.
            printf("Proceso Padre, Hijo con PID %ld reanudado\n", (long int)flag);
        }
    }
    if (flag == (pid_t)-1 && errno == ECHILD)
    {
        printf("Proceso Padre, valor de errno = %d, definido como: %s\n", errno, strerror(errno));
    }
    else
    {
        printf("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal.\n");
        exit(EXIT_FAILURE);
    }
}

// IMPLEMENTAR AQUÍ LA FUNCIÓN MANEJADORA
void funcionManejadora(int signal)
{
    printf("Soy el proceso %ld y he recibido la señal %d...\n", (long int)getpid(), signal);
    kill(getppid(), SIGUSR2);
}

void funcionManejadora2(int signal)
{
}

int main(int argc, char **argv)
{
    pid = fork();

    switch (pid)
    {
    // COMPLETAR
    case -1:
        printf("Error en fork().\n");
        exit(EXIT_FAILURE);
    case 0:
        signal(SIGUSR1, &funcionManejadora);
        printf("Soy el proceso %ld y el PID de mi padre es %ld\n", (long int)getpid(), (long int)getppid());
        printf("[HIJO]: Esperando señales...\n");
        kill(getppid(), SIGUSR2);
        while (1)
        {
            pause();
        }
    default:
        signal(SIGUSR2, &funcionManejadora2);
        printf("[PADRE]: Enviando 5 señales...\n");
        for (size_t i = 0; i < 5; i++)
        {
            pause();
            kill(pid, SIGUSR1);
        }

        pause();
        kill(pid, SIGKILL);
    }

    

    exit(EXIT_SUCCESS);
}
