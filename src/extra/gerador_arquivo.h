#ifndef gerador_arquivo_h
#define gerador_arquivo_h
#include <stdio.h>
#include <stdlib.h>

// 1
char* gerarArquivoAscendente(int numeroRegistros);

// 2
char* gerarArquivoDescendente(int numeroRegistros);

// 3
char* gerarArquivoAleatorio(int numeroRegistros);

// 4
char* gerarArquivoOrdenado(int numeroRegistros, int tipoOrdenacao);

long getRandomNumber();

int* obter20RegistrosAleatorios(FILE* arquivo, int quantidade);

#endif