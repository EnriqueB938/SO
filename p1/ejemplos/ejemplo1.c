#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int cont = 0;
    fork();
    fork();
    cont++;
    printf("Hola Mundo! El contador vale %d.\n", cont);
    exit(EXIT_SUCCESS);
}