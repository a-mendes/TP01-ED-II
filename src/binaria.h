#ifndef binaria_h
#define binaria_h

#include "extra/extra.h"
#include "extra/gerador_arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct No{
    TRegistro n;
    struct No *left, *right;
}Noh;

void binaria(int quantidade, int situacao, int chave, int opcional);
void binariaRandomica(int quantidade, int situacao, FILE* arquivo);
void printarResultado(Noh* resultado, int chave);
Noh* pesquisar(Noh *raiz, int x, OpCounter *operacoes);
Noh* criarNoh(TRegistro x, OpCounter *operacoes);
Noh* insere(Noh *raiz, TRegistro x, OpCounter *operacoes);
void finaliza(Noh* t, TRegistro* aux, FILE* arquivo);

#endif