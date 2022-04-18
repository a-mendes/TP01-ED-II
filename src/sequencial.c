#include "sequencial.h"

#include <stdio.h>
#include <stdlib.h>

int sequencial(Indice tabela[], int tamanho, Item *item, FILE *arquivo) {
    Item pagina[MAXTABELA];
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
            qntItens = (ftell(arquivo) / sizeof(Item)) % ITENSPAGINA;

            if (qntItens == 0) {
                qntItens = ITENSPAGINA;
            }
        }

        // lê a página desejada do arquivo
        position = (tabela[i - 1].chave) * ITENSPAGINA * sizeof(Item);
        position = (tabela[i - 1].chave) * ITENSPAGINA * sizeof(Item);

        fseek(arquivo, position, SEEK_SET);
        fread(&pagina, sizeof(Item), qntItens, arquivo);

        // pesquisa sequencial na página lida
        for (i = 0; i < qntItens; i++)
            if (pagina[i].chave == item->chave) {
                *item = pagina[i];
                return 1;
            }
        return 0;
    }
}