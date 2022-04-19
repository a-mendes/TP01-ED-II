#include <stdio.h>
#include <stdlib.h>

#include "b.h"
#include "b_estrela.h"
#include "binaria.h"
#include "gerador_arquivo.h"
#include "sequencial.h"

// 1 - acesso sequencial
// 2 - árvore binaria
// 3 - árvore B
// 4 - árvore B estrela

void exemploUsoGeradorArquivo() {
	//Exemplo de geração de arquivo com 50 registros
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

int main(int argc, char const *argv[]) {
    printf("TP1 - Estrutura de Dados II\n");

    printf("Digite o tipo de pesquisa desejada:\n");
    int metodo = atoi(argv[1]);

	//exemploUsoGeradorArquivo();

    // Implementar menu de acesso às pesquisas
    switch (metodo) {
        case 1:  // acesso sequencial
            // sequencial();
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