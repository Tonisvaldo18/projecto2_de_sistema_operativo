#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PIPE_SERVER "/tmp/suporte"
#define MAX_DISCIPLINAS 3
#define MAX_HORARIOS 5
#define MAX_LUGARES 10

typedef struct {
    int vagas;
    int total;
} Horario;

Horario disciplinas[MAX_DISCIPLINAS][MAX_HORARIOS];
pthread_mutex_t lock;

void inicializar_disciplinas() {
    for (int i = 0; i < MAX_DISCIPLINAS; i++) {
        for (int j = 0; j < MAX_HORARIOS; j++) {
            disciplinas[i][j].total = MAX_LUGARES;
            disciplinas[i][j].vagas = MAX_LUGARES;
        }
    }
}

void* processar_pedido(void* arg) {
    char* linha = (char*)arg;
    int aluno_inicial, num_alunos;
    char pipe_student[50];
    sscanf(linha, "%d %d %s", &aluno_inicial, &num_alunos, pipe_student);

    int inscritos = 0;
    pthread_mutex_lock(&lock);

    for (int i = 0; i < MAX_DISCIPLINAS && inscritos < num_alunos; i++) {
        for (int j = 0; j < MAX_HORARIOS && inscritos < num_alunos; j++) {
            if (disciplinas[i][j].vagas > 0) {
                disciplinas[i][j].vagas--;
                inscritos++;
            }
        }
    }

    pthread_mutex_unlock(&lock);

    int fd_student = open(pipe_student, O_WRONLY);
    write(fd_student, &inscritos, sizeof(inscritos));
    close(fd_student);

    free(linha);
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <numero_alunos>\n", argv[0]);
        return 1;
    }

    int num_alunos = atoi(argv[1]);

    // Inicializar as disciplinas e horários
    inicializar_disciplinas();
    pthread_mutex_init(&lock, NULL);

    // Abrir o named pipe existente para leitura
    FILE *fptr = fopen(PIPE_SERVER, "r");
    if (!fptr) {
        perror("Erro ao abrir o pipe do support_agent");
        return 1;
    }

    int total_inscritos = 0;
    while (total_inscritos < num_alunos) {
        char* linha = malloc(100);
        if (fgets(linha, 100, fptr) != NULL) {
            pthread_t thread;
            pthread_create(&thread, NULL, processar_pedido, linha);
            pthread_detach(thread);
            
            total_inscritos++;
        }
    }

    fclose(fptr);
    pthread_mutex_destroy(&lock);

    return 0;
}
