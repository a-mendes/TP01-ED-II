#ifndef sequencial_h
#define sequencial_h

#include <stdio.h>
#include <stdlib.h>

#include "gerador_arquivo.h"
// definir cabeçalho de função para pesquisa sequencial

#define ITENSPAGINA 20
#define MAXTABELA 100

// definição de uma entrada da tabela de índice das páginas
typedef struct {
    int chave;
} Indice;

int pesquisa(Indice *tabela, int tamanho, int quantidade, TRegistro *item, FILE *arquivo);
int sequencial(int quantidade, int situacao, int chave);

#endif
