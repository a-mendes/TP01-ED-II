#ifndef b_estrela_h
#define b_estrela_h

#include "extra/extra.h"
#include "extra/gerador_arquivo.h"

#define TAM_STRING 500                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
#define M 2  // Ordem da árvore -- número mínimo de itens por página (exceto raiz)
#define MM 4
// definir cabeçalho de função para pesquisa em arvore b_estrela
// typedef long TipoChave;

// typedef struct {
// 	int chave;
// 	long long dado1;
// 	char dado2[500]; 
// } TRegistro;


typedef enum { 
    Interna, Externa
} TipoIntExt;

typedef struct TipoPagina *TipoApontadorEstrela;

typedef struct TipoPagina {
    TipoIntExt Pt;
    union {
        struct {
            int ni;
            TRegistro ri[MM]; // duas M posições                                                  
            TipoApontadorEstrela pi[MM + 1];
        } U0;
        struct {
            int ne;
            TRegistro re[MM]; // pode ser igual, mas posso fazer com tamanho diferente de MM
        } U1;
    } UU;
} TipoPagina;


void Pesquisa(TRegistro *registro, TipoApontadorEstrela *Ap);

void Inicializa(TipoApontadorEstrela *Arvore);

void b_estrela(int quantidade, int chave, int opcional); /////

void InsereNaPagina(TipoApontadorEstrela Ap, TRegistro Reg, TipoApontadorEstrela ApDir);

void Insere(TRegistro Reg, TipoApontadorEstrela *Ap);

void Ins(TRegistro Reg, TipoApontadorEstrela Ap, int *Cresceu, TRegistro *RegRetorno, TipoApontadorEstrela *ApRetorno);

void Printa_Arvore(TipoApontadorEstrela *Arvore);

void bstar_Imprime(TipoApontadorEstrela arvore);

#endif
