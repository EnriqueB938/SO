#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) 
{
    if (argc != 2) {
        printf("Error en la linea de argumentos. \n");
        exit(EXIT_FAILURE);
    }

    FILE *fichero = fopen(argv[1], "r");
    if (fichero == NULL) {
        printf("Error al abrir el fichero");
        exit(EXIT_FAILURE);
    }

    int lineas = 0;
    char ch;
    while ((ch = fgetc(fichero)) != EOF) {
        if (ch == '\n') lineas++;
    }
    fclose(fichero);

    printf("Proceso con PID %d, PPID %d ha contado %d lineas del fichero %s.\n", getpid(), getppid(), lineas, argv[1]);

    exit(EXIT_SUCCESS);
}