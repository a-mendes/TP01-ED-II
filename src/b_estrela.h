#ifndef b_estrela_h
#define b_estrela_h

#include "extra/extra.h"
#include "extra/gerador_arquivo.h"

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
    Interna,
    Externa
} TipoIntExt;

typedef struct TipoPagina *TipoApontadorEstrela;

typedef struct TipoPagina {
    TipoIntExt Pt;
    union {
        struct {
            int ni;
            TRegistro ri[MM];  // duas M posições
            TipoApontadorEstrela pi[MM + 1];
        } U0;
        struct {
            int ne;
            TRegistro re[MM];  // pode ser igual, mas posso fazer com tamanho diferente de MM
        } U1;
    } UU;
} TipoPagina;

void bstar_Inicializa(TipoApontadorEstrela *Arvore);
void bstar_Pesquisa(TRegistro *x, TipoApontadorEstrela *Ap, int key);
void bstar_LerArquivo(FILE *file, int amount, TipoApontadorEstrela *Arvore);
void bstar_Ins(TRegistro reg, TipoApontadorEstrela Ap, short *Cresceu, TRegistro *RegRetorno, TipoApontadorEstrela *ApRetorno);
void bstar_Insere(TRegistro reg, TipoApontadorEstrela *Ap);
void bstar_InsereNaPagina(TipoApontadorEstrela Ap, TRegistro Reg, TipoApontadorEstrela ApDir);
void escreverValor(TipoApontadorEstrela *Ap);
void bstar_teste();
void bstar(int quantidade, int situacao, int chave, int opcional);
#endif
