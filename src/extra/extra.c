#include "extra.h"

#include <Windows.h>
#include <stdint.h>
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

int gettimeofday(struct timeval *tv, struct timezone *tz) {
    if (tv) {
        FILETIME filetime; /* 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601 00:00 UTC */
        ULARGE_INTEGER x;
        ULONGLONG usec;
        static const ULONGLONG epoch_offset_us = 11644473600000000ULL; /* microseconds betweeen Jan 1,1601 and Jan 1,1970 */

#if _WIN32_WINNT >= _WIN32_WINNT_WIN8
        GetSystemTimePreciseAsFileTime(&filetime);
#else
        GetSystemTimeAsFileTime(&filetime);
#endif
        x.LowPart = filetime.dwLowDateTime;
        x.HighPart = filetime.dwHighDateTime;
        usec = x.QuadPart / 10 - epoch_offset_us;
        tv->tv_sec = (time_t)(usec / 1000000ULL);
        tv->tv_usec = (long)(usec % 1000000ULL);
    }
    if (tz) {
        TIME_ZONE_INFORMATION timezone;
        GetTimeZoneInformation(&timezone);
        tz->tz_minuteswest = timezone.Bias;
        tz->tz_dsttime = 0;
    }
    return 0;
}

// SITSTEMA DE PAGINAÇÂO IMPLEMENTAR ????