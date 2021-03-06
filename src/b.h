#ifndef b_h
#define b_h

#include "extra/extra.h"

#define b_M 10
#define b_MM 20

typedef long b_TipoChave;

typedef struct b_TipoPagina *b_TipoApontador;

typedef struct b_TipoPagina {
    short n;
    TRegistro r[b_MM];
    b_TipoApontador p[b_MM + 1];
} b_TipoPagina;

typedef struct b_Bloco {
    short n;
    TRegistro r[b_MM];
    long p[b_MM + 1];
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

void b_PesquisaComTimer(int chave);

void escreverNoArquivo(FILE *arquivo, b_TipoApontador arvore, int *current_pointer, int father_pointer, OpCounter *operacoes);

void montarArquivoFromArvoreB(b_TipoApontador arvore, int opcional);

int b_PesquisaArquivo(TRegistro *x, long current_line, FILE *arquivo, OpCounter *operacoes);

void b_Free(b_TipoApontador arvore);

#endif
