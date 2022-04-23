#include "extra.h"
#include "sequencial.h"

#include <stdio.h>
#include <stdlib.h>

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

// sistema de paginação
TRegistro** sistemaPagina(int tamanho, FILE *arquivo) {
    TRegistro paginacao[20][ITENSPAGINA];

    for(int i = 0; i < 20; i++) {
        fread(paginacao[i], sizeof(TRegistro), ITENSPAGINA, arquivo); 
    }

    return paginacao;

    //ler a quantidade maxima e armazer na memoria principal
    // fread pra buscar um elemento na memoria secundaria
        // 1ª verificar no sistema de paginação
        // se tiver - busca lá 
        // se não - solicitar - ler da mem. principal 
}