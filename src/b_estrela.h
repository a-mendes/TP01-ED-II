#ifndef b_estrela_h
#define b_estrela_h

#define TAM_STRING 5000  

//definir cabeçalho de função para pesquisa em arvore b_estrela
typedef long TipoChave;

typedef struct TipoRegistro {
    TipoChave chave;
    // Pretendo mudar os nomes
    long dado1;
    char dado2[TAM_STRING];
} TipoRegistro;

typedef enum {Interna, Externa} TipoIntExt;
typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina {
    TipoIntExt Pt;
    union {
        struct {
            int ni;
            TipoChave ri[MM];
            TipoApontador pi[MM + 1];
        } U0;
        struct {
            int ne;
            TipoRegistro re[MM * 2];
        } U1;
    } UU;
} TipoPagina;

void Pesquisa(TipoRegistro *x, TipoApontador *Ap);




#endif
