#ifndef b_estrela_h
#define b_estrela_h

#include "gerador_arquivo.h"

#define TAM_STRING 5000  
// #define M 2    // ?

//definir cabeçalho de função para pesquisa em arvore b_estrela
typedef long TipoChave;

typedef enum {Interna, Externa} TipoIntExt;

typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina {
    TipoIntExt Pt;
    union {
        struct {
            int ni;
            TipoChave ri[M];
            TipoApontador pi[M + 1];
        } U0;
        struct {
            int ne;
            TRegistro re[M * 2];
        } U1;
    } UU;
} TipoPagina;

void Pesquisa(TRegistro *x, TipoApontador *Ap);




#endif
