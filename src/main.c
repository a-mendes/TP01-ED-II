#include <stdio.h>

#include "b.h"
#include "b_estrela.h"
#include "binaria.h"
#include "sequencial.h"

// 1 - acesso sequencial
// 2 - árvore binaria
// 3 - árvore B
// 4 - árvore B estrela

int main(int argc, char const *argv[]) {
    printf("TP1 - Estrutura de Dados II\n");

    printf("Digite o tipo de pesquisa desejada:\n");
    int metodo = atoi(argv[1]);

    // Implementar menu de acesso às pesquisas
    switch (metodo) {
        case 1:  // acesso sequencial
            sequencial();
            break;
        case 2:  // árvore binaria
            binaria();
            break;
        case 3:  // árvore B
            b();
            break;
        case 4:  // árvore B estrela
            b_estrela();
            break;
        default:  // opção inválida
            printf("Opção inválida!\n");
            break;
    }

    return 0;
}