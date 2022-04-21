#ifndef b_estrela_h
#define b_estrela_h

#include "gerador_arquivo.h"

#define TAM_STRING 5000  
#define M 2  // Ordem da árvore -- número mínimo de itens por página (exceto raiz)

//definir cabeçalho de função para pesquisa em arvore b_estrela
typedef long TipoChave;

typedef enum {Interna, Externa} TipoIntExt;

typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina {
    TipoIntExt Pt;
    union {
        struct {
            int ni;
            TipoChave ri[M * 2]; // dois M posições
            TipoApontador pi[M * 2 + 1]; // dois M + 1 Descendentes
        } U0;
        struct {
            int ne;
            TRegistro re[M * 2]; // 
        } U1;
    } UU;
} TipoPagina;

void Pesquisa(TRegistro *x, TipoApontador *Ap);




#endif
