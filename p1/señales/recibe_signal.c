#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void funcionManejadora(int signal)
{
    printf("He recibido la señal SIGINT(%d)...\n", signal);
}

int main()
{
    signal(SIGINT, &funcionManejadora);
    printf("Soy el proceso %ld y procedo a bloquearme para siempre...\n", (long int)getpid());
    while (1);
    exit(EXIT_SUCCESS);
}