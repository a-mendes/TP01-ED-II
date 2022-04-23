#include "extra.h"

#include <stdio.h>
#include <stdlib.h>

#include "../sequencial.h"

// imprime os registros
void printaRegistros(int tamanho, FILE *arquivo) {
    TRegistro *registro = malloc(ITENSPAGINA * sizeof(TRegistro));

    printf("Registros\n");

    int qntPag = tamanho / ITENSPAGINA;

    for (int i = 0; i < qntPag; ++i) {
        fread(registro, sizeof(TRegistro), ITENSPAGINA, arquivo);
        for (int j = 0; j < ITENSPAGINA; j++)
            printf("%-5d", registro[j].chave);
        printf("\n");
    }

    printf("\n");
    fseek(arquivo, 0, SEEK_SET);
    free(registro);
}

// SITSTEMA DE PAGINAÇÂO IMPLEMENTAR ????