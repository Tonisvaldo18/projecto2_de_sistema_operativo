#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fptr;

    // Verificar se os argumentos foram passados
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <nome_do_pipe> <palavra ou número>\n", argv[0]);
        exit(1);
    }

    char *pipe_name = argv[1];
    char *message = argv[2];

    // Abrir o named pipe para escrita
    fptr = fopen(pipe_name, "w");

    // Verificar se o pipe foi aberto corretamente
    if (fptr == NULL) {
        perror("Erro ao abrir o pipe");
        exit(1);
    }

    // Escrever a palavra ou número no pipe
    fprintf(fptr, "%s\n", message);

    // Fechar o pipe
    fclose(fptr);

    return 0;
}