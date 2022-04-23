#include "sequencial.h"

// Pré-processamento dos dados
int sequencial(int quantidade, int situacao, int chave, int opcional) {
    Indice *tabela;  // tabela de índices
    TRegistro *aux;  // item auxiliar para leitura de registros
    TRegistro item;  // chave de busca
    double tamanho;  // tamanho da tabela de índices

    // variáveis para medir o tempo de execução
    clock_t startIndice, endIndice;
    double time;

    // Gera o arquivo com a quantidade de registros informada
    char *registros = gerarArquivoAscendente(quantidade);

    // Abre o arquivo de registros
    FILE *arquivo = fopen(registros, "rb");

    // Imprime os registros
    if (opcional)
        printaRegistros(quantidade, arquivo);

    // Descobre a quantidade de páginas para alocar a tabela de índices
    if (quantidade % ITENSPAGINA == 0) {
        tamanho = quantidade / ITENSPAGINA;
    } else {
        tamanho = quantidade / ITENSPAGINA + 1;
    }

    // Aloca a tabela de índices e o auxiliar
    tabela = (Indice *)malloc(tamanho * sizeof(Indice));
    aux = (TRegistro *)malloc(ITENSPAGINA * sizeof(TRegistro));

    startIndice = clock();  // inicia a contagem do tempo

    for (int i = 0; i < tamanho; i++) {                       // percorre todas as páginas
        fread(aux, sizeof(TRegistro), ITENSPAGINA, arquivo);  // lê 20 registros por acesso (1 página)
        tabela[i].chave = aux[0].chave;                       // salva a chave do primeiro registro na tabela de indices
    }

    endIndice = clock();                                            // finaliza o cronômetro
    time = ((double)(endIndice - startIndice)) / CLOCKS_PER_SEC;    // calcula o tempo de execução
    printf("Tempo de execucao (criacao de indices): %lf\n", time);  // imprime o tempo de execução

    rewind(arquivo);  // retorna o ponteiro para o início do arquivo

    // atribui a chave procurada a uma variário do tipo TRegistro
    item.chave = chave;

    // função de busca
    if (pesquisa(tabela, tamanho, quantidade, &item, arquivo)) {
        printf("Registro encontrado!\n");
        printf("Chave: %d\n", item.chave);
        printf("Valor: %ld\n", item.dado1);
        printf("Nome: %s\n", item.dado2);
    } else {
        printf("Registro nao encontrado!\n");
        printf("Chave: %d\n", item.chave);
    }

    free(tabela);  // libera a tabela de índices
    free(aux);     // libera o auxiliar

    fclose(arquivo);  // fecha o arquivo de registros

    return 0;
}

// Busca sequencial indexada
int pesquisa(Indice *tabela, int tamanho, int quantidade, TRegistro *item, FILE *arquivo) {
    // tabela de indices - tamanho da tabela de indices - quantidade de registros - item procurado - arquivo de dados

    TRegistro *pagina;  // página de registros
    int contador = 0;   // contador de páginas
    int qntRegistros;   // quantidade de registros por página
    int position;       // posição do arquivo

    // variáveis para medir o tempo de execução
    clock_t startPesquisa, endPesquisa;
    double time;

    //  busca pela pagina onde o registro está inserido
    for (int i = 0; i < tamanho; i++) {
        if (item->chave >= tabela[i].chave)
            contador++;
    }

    if (contador == 0) {
        return 0;  // registro não encontrado
    }

    if (quantidade % ITENSPAGINA == 0) {  // se a quantidade de registros for multiplo de ITENSPAGINA, a ultima página estará completa
        qntRegistros = ITENSPAGINA;
    } else {
        if (item->chave >= tabela[tamanho - 1].chave)  // se a quantidade de registros não for multiplo de ITENSPAGINA, a ultima página não estará completa
            qntRegistros = quantidade % ITENSPAGINA;   // logo, a quantidade de registros da página será a quantidade de registros restantes
        else
            qntRegistros = ITENSPAGINA;  // o registro procurado não está na ultima página, logo a quantidade de registros da página será ITENSPAGINA
    }

    // aloca a página de registros onde contém o item
    pagina = malloc(qntRegistros * sizeof(TRegistro));

    // calcula a posição do ponteiro no arquivo
    position = (contador - 1) * ITENSPAGINA * sizeof(TRegistro);
    fseek(arquivo, position, SEEK_SET);  // posiciona o ponteiro no arquivo

    // lê os registros da página onde contém o item
    fread(pagina, sizeof(TRegistro), qntRegistros, arquivo);

    startPesquisa = clock();  // inicia a contagem do tempo

    // Utiliza da busca binária para encontrar o item procurado
    int left = 0;
    int right = qntRegistros - 1;

    while (left <= right) {
        int mid = (left + right) / 2;
        if (pagina[mid].chave == item->chave) {
            *item = pagina[mid];  // retorna o item encontrado

            endPesquisa = clock();                                                 // finaliza o cronômetro
            time = ((double)(endPesquisa - startPesquisa)) / CLOCKS_PER_SEC;       // calcula o tempo de execução
            printf("Tempo de execucao (busca sequencial indexada): %lf\n", time);  // imprime o tempo de execução

            return 1;
        } else if (pagina[mid].chave < item->chave) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    endPesquisa = clock();                                                 // finaliza o cronômetro
    time = ((double)(endPesquisa - startPesquisa)) / CLOCKS_PER_SEC;       // calcula o tempo de execução
    printf("Tempo de execucao (busca sequencial indexada): %lf\n", time);  // imprime o tempo de execução

    free(pagina);  // libera a página de registros
    return 0;
}