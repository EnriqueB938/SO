/*
        a.out
          |
          H1
          |
        / | \
       N1 N2 N3

El padre tiene un hijo, este a su vez tiene tres hijos, es decir, tres nietos y de estos cogemos el ultimo digito de cada PID y
y los sumamos dandoselo a su padre.
*/

/*Comentario Ejercicio5.c:
El segundo argumento: direccion de memoria anterior a la direccion de memoria donde se encuentre el primer argumento del programa
execvp(argv[1], &argv[2-1])

Ejercicio para hacer:
./a.out 5 f1.txt f2.txt un hijo calculo el factorial del numero y el otro abre los ficheros
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int status;
pid_t pid_hijo;

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

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Error en línea de argumentos.\n");
        exit(EXIT_FAILURE);
    }
    int N = atoi(argv[1]);

    printf("[PADRE]: soy el proceso con pid %ld y el pid de mi padre es %ld\n", (long int)getpid(), (long int)getppid());
    
    for (int i = 0; i < 1; i++)
    {
        pid_t pid = fork();
        switch (pid)
        {
        case -1:
            printf("Error en el fork().\n");
            exit(EXIT_FAILURE);
        case 0:
            printf("[HIJO]: soy el proceso con pid %ld y el pid de mi padre es %ld\n", (long int)getpid(), (long int)getppid());
            espera();
            exit(EXIT_SUCCESS);
        }

    for (int i = 0; i < 3; i++)
    {
        pid_t pid = fork();
        switch (pid)
        {
        case -1:
            printf("Error en el fork().\n");
            exit(EXIT_FAILURE);
        case 0:
            printf("[NIETOS]: soy el proceso con pid %ld y el pid de mi padre es %ld\n", (long int)getpid(), (long int)getppid());
            espera();
            exit(EXIT_SUCCESS);
        }
    }
    }
}   