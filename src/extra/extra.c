#include "extra.h"

#include <Windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../sequencial.h"

// imprime os registros
void printaRegistros(int tamanho, FILE *arquivo) {
    rewind(arquivo);  // volta o ponteiro do arquivo para o início

    TRegistro *registro = malloc(ITENSPAGINA * sizeof(TRegistro));

    int qntPag;

    printf("Registros\n");

    if (tamanho % ITENSPAGINA == 0) {
        qntPag = tamanho / ITENSPAGINA;
    } else {
        qntPag = tamanho / ITENSPAGINA + 1;
    }

    for (int i = 0; i < qntPag; ++i) {
        fread(registro, sizeof(TRegistro), ITENSPAGINA, arquivo);

        if (i == qntPag - 1 && tamanho % ITENSPAGINA != 0) {
            for (int j = 0; j < tamanho % ITENSPAGINA; j++)
                printf("%d\t", registro[j].chave);
        }

        else {
            for (int j = 0; j < ITENSPAGINA; j++)
                printf("%d\t", registro[j].chave);
        }
        printf("\n");
    }

    printf("\n");
    rewind(arquivo);
    free(registro);
}

// Função para obter o tempo atual em milissegundos
int gettimeofday(struct timeval *tv, struct t_timezone *tz) {
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
        TIME_ZONE_INFORMATION t_timezone;
        GetTimeZoneInformation(&t_timezone);
        tz->tz_minuteswest = t_timezone.Bias;
        tz->tz_dsttime = 0;
    }
    return 0;
}

// SITSTEMA DE PAGINAÇÂO IMPLEMENTAR ????