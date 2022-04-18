#ifndef b_h
#define b_h

// definir cabeçalho de função para pesquisa em arvore b
#define M 1
#define MM 2

typedef long b_TipoChave;

typedef struct b_TipoRegistro {
    b_TipoChave Chave;
    int valor;
} b_TipoRegistro;

typedef struct b_TipoPagina *b_TipoApontador;

typedef struct b_TipoPagina {
    short n;
    b_TipoRegistro r[MM];
    b_TipoApontador p[MM + 1];
} b_TipoPagina;

// funções

void b_Inicializa(b_TipoApontador Arvore);

void b_Pesquisa(b_TipoRegistro *x, b_TipoApontador Ap);

void b_Imprime(b_TipoApontador arvore);

void b_InsereNaPagina(b_TipoApontador Ap, b_TipoRegistro Reg, b_TipoApontador ApDir);

void b_Ins(b_TipoRegistro Reg, b_TipoApontador Ap, short *Cresceu, b_TipoRegistro *RegRetorno, b_TipoApontador *ApRetorno);

void b_Insere(b_TipoRegistro Reg, b_TipoApontador *Ap);

void b_Reconstitui(b_TipoApontador ApPag, b_TipoApontador ApPai, int PosPai, short *Diminuiu);

void b_Antecessor(b_TipoApontador Ap, int Ind, b_TipoApontador ApPai, short *Diminuiu);

void b_Ret(b_TipoChave Ch, b_TipoApontador *Ap, short *Diminuiu);

void b_Retira(b_TipoChave Ch, b_TipoApontador *Ap);

#endif
