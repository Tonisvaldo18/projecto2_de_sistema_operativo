#!/bin/bash

NAMEDPIPE="/tmp/suporte"

# 0 - Boas Vindas

echo "===== B E N V I N D O AO S I G A ====="

# 1 - Verificar se existe o named pipe, se não existir, criar 
if [[ ! -p $NAMEDPIPE ]]; then
    mkfifo "$NAMEDPIPE"
    echo "Foi criado o $NAMEDPIPE"
fi

# 2 - Executar 3 students em background com argumentos
for i in {1..3}; do
    echo "Executando student $i com $i-ºPedido"
    ./student "$NAMEDPIPE" "$i-ºPedido" &> &
done

# 3 - Executar o suporte agente com o named pipe
echo "Executando suporte agente"
./suporte_agente.sh "$NAMEDPIPE" &

# 4 - Esperar 1 segundo
sleep 1

# 5 - Mandar o texto 'quit' para o named pipe
echo "Enviando comando 'quit' para o pipe"
echo 'quit' > "$NAMEDPIPE"

# 6 - Terminar todos os processos
wait
echo "»»»»»» T O D O S P R O C E S S O S F I N A L I Z A D O S. ««««««."

# 7 - Remover o named pipe
echo "==== REMOVENDO O $NAMEDPIPE ====."
rm "$NAMEDPIPE"
