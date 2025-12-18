/*
Estos son los nombres de las señales que hay:
SIGINT: Es el único que tiene un atajo equivalente CONTROL C, se pira pero sin liberar la memoria.
SIGTERM: Se pira liberando memoria.
SIGKILL: Mata al bucle infinito el cual esta transmitiendo una señal o que esta haciendo un proceso.
SIGUSR1:|
        |--->Estas son señales que puedes definir tu su funcionalidad.
SIGUSR2:|
(Todas las señales tienen una funcionalidad asociada que se puede redefinir, salvo SIGUSR1 Y SIGUSR2)
*/


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
    int pid;
    printf("Introduce el PID del proceso al que deseas enviar la señal SIGKILL: ");
    scanf("%d", &pid);
    kill(pid, SIGKILL);
    exit(EXIT_SUCCESS);
}