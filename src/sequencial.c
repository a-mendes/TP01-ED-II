#include "sequencial.h"

// Pré-processamento dos dados
int sequencial(int quantidade, int situacao, int chave) {
    Indice *tabela;  // tabela de índices
    TRegistro *aux;  // item do arquivo de registros
    TRegistro item;  // chave de busca
    double size;     // tamanho da tabela de índices

    // Gera o arquivo de dados com a quantidade de registros informada
    char *registros = gerarArquivoAscendente(quantidade);

    // Abre o arquivo de dados
    FILE *arquivo = fopen(registros, "rb");

    // Descobre a quantidade de páginas para alocar a tabela de índices
    if (quantidade % ITENSPAGINA == 0) {
        size = quantidade / ITENSPAGINA;
    } else {
        size = quantidade / ITENSPAGINA + 1;
    }

    // Aloca a tabela de índices e o vetor de registros
    tabela = (Indice *)malloc(size * sizeof(Indice));
    aux = (TRegistro *)malloc(ITENSPAGINA * sizeof(TRegistro));

    for (int i = 0; i < size; i++) {                                 // percorre todos os registros
        fread(&aux, sizeof(TRegistro), (ITENSPAGINA - 1), arquivo);  // lê os 20 registros (1 a 20)
        tabela[i].chave = aux[0].chave;                              // salva a chave do primeiro registro na tabela de indices
    }

    item.chave = chave;

    // função de busca
    if (pesquisa(tabela, size, &item, arquivo)) {
        printf("Registro encontrado!\n");
        printf("Chave: %d\n", item.chave);
        printf("Valor: %s\n", item.dado1);
        printf("Nome: %s\n", item.dado2);
    } else{
        printf("Registro não encontrado!\n");
        printf("Chave: %d\n", item.chave);
    }

    return 0;
}

// Busca sequencial indexada
int pesquisa(Indice tabela[], int tamanho, TRegistro *item, FILE *arquivo) {
    TRegistro pagina[MAXTABELA];
    int i = 0;
    int qntItens;
    long position;

    //  busca pela pagina onde o item pode estar inserido
    while (i < tamanho && item->chave >= tabela[i].chave) i++;

    // se i = 0 então o item não se encontra na página. (item < 1º item da página)
    if (i == 0) {
        return 0;
    } else {
        // a ultima página pode não estar completa
        if (i < tamanho) {
            qntItens = ITENSPAGINA;
        } else {
            fseek(arquivo, 0, SEEK_END);
            qntItens = (ftell(arquivo) / sizeof(TRegistro)) % ITENSPAGINA;

            if (qntItens == 0) {
                qntItens = ITENSPAGINA;
            }
        }

        // lê a página desejada do arquivo
        position = (tabela[i - 1].chave) * ITENSPAGINA * sizeof(TRegistro);
        position = (tabela[i - 1].chave) * ITENSPAGINA * sizeof(TRegistro);

        fseek(arquivo, position, SEEK_SET);
        fread(&pagina, sizeof(TRegistro), qntItens, arquivo);

        // pesquisa sequencial na página lida
        for (i = 0; i < qntItens; i++)
            if (pagina[i].chave == item->chave) {
                *item = pagina[i];
                return 1;
            }
        return 0;
    }
}