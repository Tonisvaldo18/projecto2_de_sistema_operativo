#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>  // Cabeçalho necessário para mkfifo

#define PIPE_SERVER "/tmp/suporte"
#define PIPE_TEMPLATE "/tmp/student_%d"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Uso: %s <numero_student> <aluno_inicial> <num_alunos>\n", argv[0]);
        exit(1);
    }

    int numero_student = atoi(argv[1]);
    int aluno_inicial = atoi(argv[2]);
    int num_alunos = atoi(argv[3]);

    // Exibir informações iniciais
    printf("student %d: aluno inicial=%d, número de alunos=%d\n", numero_student, aluno_inicial, num_alunos);

    // Criar o named pipe do student para receber a resposta do support_agent
    char pipe_student[50];
    sprintf(pipe_student, PIPE_TEMPLATE, numero_student);
    
    // Remover o pipe do student se ele já existir
    if (access(pipe_student, F_OK) == 0) {
        unlink(pipe_student);
    }
    
    if (mkfifo(pipe_student, 0666) == -1)
    {
        perror("Erro ao criar o pipe do student");
        exit(1);
    }

    // Abrir o named pipe do support_agent para enviar o pedido
    FILE *fptr = fopen(PIPE_SERVER, "w");
    if (fptr == NULL)
    {
        perror("Erro ao abrir o pipe do support_agent");
        unlink(pipe_student);
        exit(1);
    }

    // Enviar o pedido formatado ao support_agent
    fprintf(fptr, "%d %d %s\n", aluno_inicial, num_alunos, pipe_student);
    fclose(fptr);

    // Abrir o pipe do student para ler a resposta do support_agent
    FILE *fptr_response = fopen(pipe_student, "r");
    if (fptr_response == NULL)
    {
        perror("Erro ao abrir o pipe do student para leitura");
        unlink(pipe_student);
        exit(1);
    }

    int numero_alunos_inscritos;
    fscanf(fptr_response, "%d", &numero_alunos_inscritos);
    fclose(fptr_response);

    // Exibir a quantidade de alunos inscritos
    printf("student %d: alunos inscritos=%d\n", numero_student, numero_alunos_inscritos);

    // Remover o pipe do student
    unlink(pipe_student);

    return 0;
}
