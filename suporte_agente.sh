#!/bin/bash

# Verificar se o nome do pipe foi passado como argumento
if [[ -z "$1" ]]; then
  echo "Erro: O nome do named pipe não foi fornecido."
  exit 1
fi

NAMEDPIPE="$1"

# Se o pipe ainda não existir, aguardar sua criação
while [[ ! -p $NAMEDPIPE ]]; do
  echo "Aguardando a criação do named pipe: $NAMEDPIPE..."
  sleep 1
done

echo "Named pipe encontrado: $NAMEDPIPE"

# Loop contínuo para ler e processar o named pipe
while true; do
  # Lê uma linha do named pipe
  if read -r line < "$NAMEDPIPE"; then
    echo "Recebido: $line"

    # Se a mensagem for 'quit', encerra o script
    if [[ "$line" == "quit" ]]; then
      echo "Comando 'quit' recebido. Encerrando..."
      exit 0
    fi

    # Caso contrário, imprime a mensagem de processamento
    echo "Processando pedido: $line"

    # Espera aleatória entre 1 e 5 segundos
    sleep $((RANDOM % 5 + 1))
  fi
done
