#ifndef b_estrela_h
#define b_estrela_h

#include "extra/gerador_arquivo.h"
#include "extra/extra.h"

#define TAM_STRING 5000  
#define ME 2  // Ordem da árvore -- número mínimo de itens por página (exceto raiz)

//definir cabeçalho de função para pesquisa em arvore b_estrela
typedef long TipoChave;

typedef enum {Interna, Externa} TipoIntExt;

typedef struct TipoPagina* TipoApontadorEstrela;

typedef struct TipoPagina {
    TipoIntExt Pt;
    union {
        struct {
            int ni;
            TipoChave ri[ME * 2]; // dois M posições
            TipoApontadorEstrela pi[ME * 2 + 1]; // dois M + 1 Descendentes
        } U0;
        struct {
            int ne;
            TRegistro re[ME * 2]; // 
        } U1;
    } UU;
} TipoPagina;

void Pesquisa(TRegistro *x, TipoApontadorEstrela *Ap);

#endif
