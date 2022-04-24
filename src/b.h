#ifndef b_h
#define b_h

#include "extra/extra.h"

#define M 2
#define MM 4

typedef long b_TipoChave;

typedef struct b_TipoPagina *b_TipoApontador;

typedef struct b_TipoPagina {
    short n;
    TRegistro r[MM];
    b_TipoApontador p[MM + 1];
} b_TipoPagina;

typedef struct b_Bloco {
    short n;
    TRegistro r[MM];
    long p[MM + 1];
} b_Bloco;

// funções

void b_Inicializa(b_TipoApontador *Arvore);

int b_Pesquisa(TRegistro *x, b_TipoApontador Ap);

void b_Imprime(b_TipoApontador arvore);

void b_InsereNaPagina(b_TipoApontador Ap, TRegistro Reg, b_TipoApontador ApDir);

void b_Ins(TRegistro Reg, b_TipoApontador Ap, short *Cresceu, TRegistro *RegRetorno, b_TipoApontador *ApRetorno);

void b_Insere(TRegistro Reg, b_TipoApontador *Ap);

void b_Reconstitui(b_TipoApontador ApPag, b_TipoApontador ApPai, int PosPai, short *Diminuiu);

void b_Antecessor(b_TipoApontador Ap, int Ind, b_TipoApontador ApPai, short *Diminuiu);

void b_Ret(b_TipoChave Ch, b_TipoApontador *Ap, short *Diminuiu);

void b_Retira(b_TipoChave Ch, b_TipoApontador *Ap);

int b(int quantidade, int situacao, int chave, int opcional);

b_TipoApontador montarArvoreBFromArquivo(int quantidade, FILE *arquivo);

void b_PesquisaComTimer(int chave, b_TipoApontador arvore);

void escreverNoArquivo(FILE *arquivo, b_TipoApontador arvore, int *current_pointer, int father_pointer);

void montarArquivoFromArvoreB(b_TipoApontador arvore);
#endif
