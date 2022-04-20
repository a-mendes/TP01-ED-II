#include "sequencial.h"

#include <string.h>

// Pré-processamento dos dados
int sequencial(int quantidade, int situacao, int chave) {
    Indice *tabela;              // tabela de índices
    TRegistro aux[ITENSPAGINA];  // item auxiliar para leitura de registros
    TRegistro item;              // chave de busca
    double tamanho;              // tamanho da tabela de índices

    // Gera o arquivo de dados com a quantidade de registros informada
    char *registros = gerarArquivoAscendente(quantidade);

    // Abre o arquivo de dados
    FILE *arquivo = fopen(registros, "rb");

    // Descobre a quantidade de páginas para alocar a tabela de índices
    if (quantidade % ITENSPAGINA == 0) {
        tamanho = quantidade / ITENSPAGINA;
    } else {
        tamanho = quantidade / ITENSPAGINA + 1;
    }

    // Aloca a tabela de índices
    tabela = (Indice *)malloc(tamanho * sizeof(Indice));

    for (int i = 0; i < tamanho; i++) {                              // percorre todas as páginas
        fread(&aux, sizeof(TRegistro), (ITENSPAGINA - 1), arquivo);  // lê os 20 registros da página
        tabela[i].chave = aux[0].chave;                              // salva a chave do primeiro registro na tabela de indices
    }

    fseek(arquivo, 0, SEEK_SET);  // retorna o ponteiro para o início do arquivo

    item.chave = chave;

    // função de busca
    if (pesquisa(tabela, tamanho, quantidade, &item, arquivo)) {
        printf("Registro encontrado!\n");
        printf("Chave: %d\n", item.chave);
        printf("Valor: %s\n", item.dado1);
        printf("Nome: %s\n", item.dado2);
    } else {
        printf("Registro nao encontrado!\n");
        printf("Chave: %d\n", item.chave);
    }

    return 0;
}

// Busca sequencial indexada
int pesquisa(Indice *tabela, int tamanho, int quantidade, TRegistro *item, FILE *arquivo) {
    TRegistro *pagina;  // página de registros
    int contador = 0;   // contador de páginas
    int qntItens;       // quantidade de itens na página
    int position;       // posição do arquivo

    //  busca pela pagina onde o item está inserido
    for (int i = 0; i < tamanho; i++)
        if (item->chave >= tabela[i].chave)
            contador++;

    if (contador == 0) return 0; // item não encontrado 

    // a ultima página pode não estar completa
    // Alert: REFATORAR
    if (quantidade % ITENSPAGINA == 0) { // todas as paginas tem o mesmo tamanho
        qntItens = ITENSPAGINA;
    } else {
        if (item->chave >= tabela[tamanho - 1].chave)
            qntItens = quantidade % ITENSPAGINA;
        else
            qntItens = ITENSPAGINA;
    }

    pagina = (TRegistro *)malloc(qntItens * sizeof(TRegistro));

    // lê a página desejada do arquivo
    position = contador * ITENSPAGINA * sizeof(TRegistro);

    fseek(arquivo, position, SEEK_SET);
    fread(&pagina, sizeof(TRegistro), 1, arquivo);

    // pesquisa sequencial na página lida
    // Alert: REFATORAR -> busca binária.
    for (int i = 0; i < qntItens; i++) {
        printf("%d\n", pagina[i].chave);
        if (pagina[i].chave == item->chave) {
            *item = pagina[i];
            return 1;
        }
    }

    return 0;
}