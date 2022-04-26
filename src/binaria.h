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
void binariaRandomica(int quantidade, int situacao, int chave, int opcional);
void printarResultado(Noh* resultado, int chave);
Noh* pesquisar(Noh *raiz, int x);
Noh* criarNoh(TRegistro x);
Noh* insere(Noh *raiz, TRegistro x);

#endif