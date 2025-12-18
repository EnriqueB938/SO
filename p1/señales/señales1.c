/*
Creamos un padre que tiene un hijo, tenmos que hacer que el hijo y el padre se presenten y en cuanto lo hagan, es decir, que reciba una señal,
hay que matar el proceso. EL padre le tiene que enviar 5 señales SIGUSR1 en intervalos de 1 segundo (sleep(1)); y despues de las 5 señales
mandar una señal SIGKILL
while(1)
{
    pause();
}
*/

/*
Hacer que este ejercico guncione sin sleeps, es decir, utilizando un pause, utilizando el kill y el pause para que funcione. Utilizamos 
pause que deja de estar parado cuando recibe la señal del hijo, y en este momento enviar la señal SIGUSR1.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

int status;
pid_t pid_hijo;

void funcionManejadora(int signal)
{
    printf("Soy %ld y he recibido la señal SIGUSR1(%d)...\n", (long int)getpid(), signal);
}


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

int main()
{
    pid_t pid = fork();

    if (pid == -1)
    {
        printf("Error en el fork().\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        signal(SIGUSR1, &funcionManejadora);
        printf("[HIJO]: soy el proceso con pid %ld y el pid de mi padre es %ld\n", (long int)getpid(), (long int)getppid());
        while(1)
        {
            pause();
        }
        exit(EXIT_SUCCESS);
    }
    else
    {
        printf("[PADRE]: soy el proceso con pid %ld y el pid de mi padre es %ld\n", (long int)getpid(), (long int)getppid());
        for(int i=0;i<5;i++)
        {
            sleep(1);
            kill(pid, SIGUSR1);
        }
        printf("[PADRE]: Ya envié las 5 señales, ahora voy a matar al hijo.\n");

        sleep(1);
        kill(pid, SIGKILL);
    }
    
    espera();
    exit(EXIT_FAILURE);

}




