#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "b.h"
#include "b_estrela.h"
#include "binaria.h"
#include "gerador_arquivo.h"
#include "sequencial.h"

void exemploUsoGeradorArquivo() {
    // Exemplo de geração de arquivo com 50 registros
    char *nomeArquivo = gerarArquivoDescendente(50);
    FILE *file;
    file = fopen(nomeArquivo, "rb");
    TRegistro registros[50];
    fread(registros, sizeof(TRegistro), 50, file);
    for (int i = 0; i < 50; ++i) {
        printf("%d\t", registros[i].chave);
        printf("%lld\t", registros[i].dado1);
        printf("%s\n", registros[i].dado2);
    }
}

// ./TP1.exe (metodo) (quantidade) (situacao) (chave)

int main(int argc, char const *argv[]) {
    printf("TP1 - Estrutura de Dados II\n");
    // fazer uma lista com cada registro [20] existentes em um file pré definido

    char pesquisa[8];                // chamada do método
    strcpy(pesquisa, argv[1]);       // copia o método de pesquisa

    int metodo = atoi(argv[2]);      // metodo de pesquisa a ser executado (1 - sequencial, 2 - binaria, 3 - B, 4 - B estrela)
    int quantidade = atoi(argv[3]);  // Quantidade de registros do arquivo (100, 200, 2k, 20k, 200k, 2kk)
    int situacao = atoi(argv[4]);    // situação de ordem do arquivo (1 - Crescente, 2 - Decrescente, 3 - Desordenado)
    int chave = atoi(argv[5]);       // chave a ser pesquisada no arquivo 

    // Implementar menu de acesso às pesquisas
    switch (metodo) {
        case 1:  // acesso sequencial
            printf("### Pesquisa sequencial ###\n");

            if (strcmp(pesquisa, "pesquisa") == 0) {
                sequencial(quantidade, situacao, chave);
            } else {
                for (int i = 0; i < 20; i++) {
                    printf("Pesquisa de numero %d\n", i + 1);

                    chave = rand() % 1000;
                    printf("Chave: %d\n", chave);

                    sequencial(quantidade, situacao, chave);
                    printf("===============================\n");
                }
            }

            break;
        case 2:  // árvore binaria
            // binaria();
            break;
        case 3:  // árvore B
            // b();
            break;
        case 4:  // árvore B estrela
            // b_estrela();
            break;
        default:  // opção inválida
            printf("Opcao invalida!\n");
            break;
    }

    return 0;
}