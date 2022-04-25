#ifndef extra_h
#define extra_h

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#define ITENSPAGINA 20

typedef struct {
    int chave;
    long long dado1;
    char dado2[50];
} TRegistro;

void printaRegistros(int tamanho, FILE *arquivo);

struct t_timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

typedef struct OpCounter {
    long transfers;
    long comparisons;
} OpCounter;

int gettimeofday(struct timeval *tv, struct t_timezone *tz);

#endif