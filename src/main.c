#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "b.h"
#include "b_estrela.h"
#include "binaria.h"
#include "extra/extra.h"
#include "extra/gerador_arquivo.h"
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

// argumentos == ./TP1.exe pesquisa (Método) (Quantidade de registros) (Situação) (Chave) (Opcional)

int main(int argc, char const *argv[]) {
    printf("TP1 - Estrutura de Dados II\n");

    srand(time(NULL));

    int metodo = -1;      // metodo de pesquisa a ser executado (1 - sequencial, 2 - binaria, 3 - B, 4 - B estrela)
    int quantidade = -1;  // quantidade de registros do arquivo (100, 200, 2k, 20k, 200k, 2kk)
    int situacao = 1;     // situação de ordem do arquivo (1 - Crescente, 2 - Decrescente, 3 - Desordenado)
    int chave = -1;       // chave a ser pesquisada no arquivo
    int opcional = 0;     // parâmetro opcional (1 - imprime os registros, 0 - não imprime os registros)

    if (argv[1] != NULL) {
        metodo = atoi(argv[1]);

        if (argv[2] != NULL ) {
            quantidade = atoi(argv[2]);

            if (argv[3] != NULL && situacao > 0 && situacao < 4) {
                situacao = atoi(argv[3]);

                if (argv[4] != NULL && argv[4] > 0) {
                    chave = atoi(argv[4]);

                    if (argv[5] != NULL) {
                        opcional = atoi(argv[5]);
                    }
                }
            }
        }
    }


    // Implementar menu de acesso às pesquisas
    switch (metodo) {
        case 1:  // acesso sequencial
            printf("### Pesquisa sequencial ###\n");

            sequencial(quantidade, situacao, chave, opcional);
            break;
        case 2:  // árvore binaria
            printf("### Pesquisa Binaria ###\n");
            
            binaria(quantidade, situacao, chave, opcional);

            break;
        case 3:  // árvore B
            printf("### Pesquisa arvore B ###\n");

            b(quantidade, situacao, chave, opcional);

            break;
        case 4:  // árvore B estrela
            printf("### Arvore B* ###\n");
            bstar(quantidade, situacao, chave, opcional);
            // bstar_teste();
            // b_estrela(quantidade, chave, opcional);
            //  ./TP1.exe pesquisa (Método) (Quantidade de registros) (Situação) (Chave) (Opcional)

            break;
        default:  // opção inválida
            printf("Opcao invalida!\n");
            break;
    }

    return 0;
}