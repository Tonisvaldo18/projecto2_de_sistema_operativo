#!/bin/bash

NAMEDPIPE="/tmp/suporte"
NALUN=30  # Número total de alunos para inscrição
NDISCIP=3  # Número de disciplinas
NLUG=10  # Número de lugares por sala
NSTUD=3  # Número de processos student

# 1 - Exibir mensagem de boas-vindas e limpar a tela
echo "===[ B E M - V I N D O  A O  S I G A ]==="
sleep 2
clear

# 2 - Remover o named pipe se já existir e então criar um novo
if [[ -p $NAMEDPIPE ]]; then
    echo "Removendo named pipe existente: $NAMEDPIPE"
    rm "$NAMEDPIPE"
fi

echo "Criando named pipe em $NAMEDPIPE"
mkfifo "$NAMEDPIPE"

# 3 - Calcular o número máximo de horários por disciplina
NHOR=$(( (NALUN + NLUG - 1) / NLUG ))

# 4 - Executar o support_agent em background, passando NALUN como argumento
echo "Executando support_agent com $NALUN alunos"
./support_agent "$NALUN" &

# 5 - Dividir o número de alunos entre os processos student e executar cada um em background
for i in $(seq 1 $NSTUD); do
    ALUNO_INICIAL=$(( (i - 1) * (NALUN / NSTUD) ))
    NUM_ALUNOS=$((NALUN / NSTUD))
    echo "Executando student $i: aluno inicial=$ALUNO_INICIAL, número de alunos=$NUM_ALUNOS"
    ./student "$i" "$ALUNO_INICIAL" "$NUM_ALUNOS" &
done

# 6 - Esperar até todos os processos (students e support_agent) terminarem
wait

# 7 - Remover o named pipe
echo "Removendo named pipe $NAMEDPIPE"
rm "$NAMEDPIPE"

echo "Todos os processos finalizaram."
