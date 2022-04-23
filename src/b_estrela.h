#ifndef b_estrela_h
#define b_estrela_h

#include "extra/extra.h"
#include "extra/gerador_arquivo.h"

#define TAM_STRING 5000
#define ORDEM 2  // Ordem da árvore -- número mínimo de itens por página (exceto raiz)

// definir cabeçalho de função para pesquisa em arvore b_estrela
typedef long Chave;

typedef enum { Interna,
               Externa } TipoIntExt;

typedef struct TipoPagina *ApontadorEstrela;

typedef struct TipoPagina {
    TipoIntExt NosOuFolha; // onde interno == Nó e Externo == Folha
    union {
        struct {
            int numeroChaves;
            Chave chave[ORDEM * 2];                          // dois M posições
            ApontadorEstrela ponteiroTree[ORDEM * 2 + 1];  // dois M + 1 Descendentes
        } NOS;
        struct {
            int numeroRegistros;
            TRegistro registro[ORDEM * 2];  //
        } FOLHAS;
    } ESTRUTURA;
} TipoPagina;

void Pesquisa(TRegistro *registro, ApontadorEstrela *Ap);

void Inicializa(ApontadorEstrela *Arvore);

#endif