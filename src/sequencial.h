#ifndef sequencial_h
#define sequencial_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "extra/gerador_arquivo.h"
#include "extra/extra.h"

// definição de uma entrada da tabela de índice das páginas
typedef struct {
    int chave;
} Indice;

int pesquisaSequencial(int quantidade, int situacao, int chave, int opcional);
int preProcessamento(int quantidade, int situacao, int chave, int opcional);
int pesquisa(Indice *tabela, int tamanho, int quantidade, TRegistro *item, FILE *arquivo);

#endif
