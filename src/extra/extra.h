#ifndef extra_h
#define extra_h

#include <stdio.h>
#include <stdlib.h>

#define ITENSPAGINA 20

typedef struct {
	int chave;
	long long dado1;
	char dado2[5000]; 
} TRegistro;

void printaRegistros(int tamanho, FILE *arquivo);

#endif