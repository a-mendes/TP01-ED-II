#include "sequencial.h"

#include <string.h>

// Pré-processamento dos dados
int sequencial(int quantidade, int situacao, int chave) {
    Indice *tabela;              // tabela de índices
    TRegistro aux[ITENSPAGINA];  // item auxiliar para leitura de registros
    TRegistro item;              // chave de busca
    double tamanho;              // tamanho da tabela de índices

    // Gera o arquivo com a quantidade de registros informada
    char *registros = gerarArquivoAscendente(quantidade);
    printf("Pesquisa sequencial indexada\n");

    // Abre o arquivo de registros
    FILE *arquivo = fopen(registros, "rb");

    // Descobre a quantidade de páginas para alocar a tabela de índices
    if (quantidade % ITENSPAGINA == 0) {
        tamanho = quantidade / ITENSPAGINA;
    } else {
        tamanho = quantidade / ITENSPAGINA + 1;
    }

    fseek(arquivo, 0, SEEK_SET);  // retorna o ponteiro para o início do arquivo

    // Aloca a tabela de índices
    tabela = (Indice *)malloc(tamanho * sizeof(Indice));

    for (int i = 0; i < tamanho; i++) {                              // percorre todas as páginas
        fread(&aux, sizeof(TRegistro), (ITENSPAGINA - 1), arquivo);  // lê 20 registros por acesso (1 página)
        tabela[i].chave = aux[0].chave;                              // salva a chave do primeiro registro na tabela de indices
    }

    fseek(arquivo, 0, SEEK_SET);  // retorna o ponteiro para o início do arquivo

    // atribui a chave procurada a uma variário do tipo TRegistro
    item.chave = chave;

    // função de busca
    if (pesquisa(tabela, tamanho, quantidade, &item, arquivo)) {
        printf("Registro encontrado!\n");
        printf("Chave: %d\n", item.chave);
        printf("Valor: %d\n", item.dado1);
        printf("Nome: %s\n", item.dado2);
    } else {
        printf("Registro nao encontrado!\n");
        printf("Chave: %d\n", item.chave);
    }

    return 0;
}

// Busca sequencial indexada
int pesquisa(Indice *tabela, int tamanho, int quantidade, TRegistro *item, FILE *arquivo) {
    // tabela de indices - tamanho da tabela de indices - quantidade de registros - item procurado - arquivo de dados

    TRegistro *pagina;  // página de registros
    int contador = 0;   // contador de páginas
    int qntItens;       // quantidade de registros por página
    int position;       // posição do arquivo

    //  busca pela pagina onde o registro está inserido
    for (int i = 0; i < tamanho; i++)
        if (item->chave >= tabela[i].chave)
            contador++;

    if (contador == 0) return 0;  // registro não encontrado

    if (quantidade % ITENSPAGINA == 0) {  // se a quantidade de registros for multiplo de 20, a ultima página estará completa
        qntItens = ITENSPAGINA;
    } else {                                           // se a quantidade de registros for impar, a ultima página não estará completa
        if (item->chave >= tabela[tamanho - 1].chave)  // logo, a quantidade de registros da página será a quantidade de registros restantes
            qntItens = quantidade % ITENSPAGINA;
        else
            qntItens = ITENSPAGINA;  // se não a quantidade de registros na última página é 20
    }

    // aloca a página de registros onde contém o item
    pagina = (TRegistro *)malloc(qntItens * sizeof(TRegistro));

    // calcula a posição do ponteiro no arquivo
    position = (contador - 1) * ITENSPAGINA * sizeof(TRegistro);
    fseek(arquivo, position, SEEK_SET);  // posiciona o ponteiro no arquivo

    // lê os registros da página onde contém o item
    for (int i = 0; i < 1; i++) {
        fread(&pagina[i], sizeof(TRegistro), 20, arquivo);
    }

    // Utiliza da busca binária para encontrar o item procurado
    int left = 0;
    int right = qntItens - 1;
    int mid;

    while (left <= right) {
        mid = (left + right) / 2;
        printf("%d\n", mid);
        if (pagina[mid].chave == item->chave) {
            *item = pagina[mid];
            return 1;
        } else if (pagina[mid].chave < item->chave) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return 0;
}