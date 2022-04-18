#ifndef sequencial_h
#define sequencial_h

//definir cabeçalho de função para pesquisa sequencial

#define ITENSPAGINA 4
#define MAXTABELA 100

// definição de uma entrada da tabela de índice das páginas
typedef struct {
    int chave;
} Indice;

// definição de um item do arquivo de dados
typedef struct {
    int chave;
    int valor;
    char string[500];
} Item;

int pesquisa(Indice tabela[], int tamanho, Item *item, FILE *arquivo);

#endif
