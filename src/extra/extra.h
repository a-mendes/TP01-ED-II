#ifndef extra_h
#define extra_h

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#define ITENSPAGINA 20

typedef struct {
    int chave;
    long long dado1;
    char dado2[5000];
} TRegistro;

void printaRegistros(int tamanho, FILE *arquivo);

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

int gettimeofday(struct timeval *tv, struct timezone *tz);

#endif