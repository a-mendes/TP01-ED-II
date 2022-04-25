#include "gerador_arquivo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "extra.h"

/**
 *  arquivos contendo 100, 200, 2.000, 20.000, 200.000 e 2.000.000 registros
 * arquivos ordenados ascendentemente, descendentemente e aleatoriamente
 */

void montarNomeArquivo(char *nomeArquivo, int numeroRegistros, int situacao) {
    /** Definindo nome do arquivo
     * 		"<pasta>/<tipo_de_ordenação>_<quantidade_de_registros>.dat"
     */
    strcpy(nomeArquivo, "data/");

    static char numeroRegistrosString[10];
    itoa(numeroRegistros, numeroRegistrosString, 10);  // Converte int para string

    static char situacaoOrdenacao[15];
    switch (situacao) {
        case 1:
            strcpy(situacaoOrdenacao, "ascendente_");
            break;
        case 2:
            strcpy(situacaoOrdenacao, "descendente_");
            break;

        case 3:
            strcpy(situacaoOrdenacao, "aleatorio_");
            break;
    }

    strcat(nomeArquivo, situacaoOrdenacao);
    strcat(nomeArquivo, numeroRegistrosString);
    strcat(nomeArquivo, ".dat");
}

// Usado para ordenar o arquivo
void ShellSort(TRegistro *registros, int n, int tipoOrdenacao) {
    int h;

    for (h = 1; h < n; h = 3 * h + 1)
        ;  // h inicial

    do {
        h = (h - 1) / 3;

        for (int i = h; i < n; i++) {
            TRegistro aux = registros[i];
            int j = i;

            // Ascendente
            if (tipoOrdenacao == 1) {
                while (registros[j - h].chave > aux.chave) {
                    registros[j] = registros[j - h];

                    j = j - h;

                    if (j < h)
                        break;
                }
            }

            // Descendente
            else if (tipoOrdenacao == 2) {
                while (registros[j - h].chave < aux.chave) {
                    registros[j] = registros[j - h];

                    j = j - h;

                    if (j < h)
                        break;
                }
            }

            registros[j] = aux;
        }
    } while (h != 1);
}

char *gerarArquivoOrdenado(int numeroRegistros, int tipoOrdenacao) {
    /** Verifica se já existe um arquivo aleatório para a quanidade de registros solicitada
     * 		Se sim, ordenar o arquivo já existente
     * 		Se não, criar um arquivo aleatório e ordená-lo
     */

    FILE *arquivoAleatorio;
    char nomeArquivoAleatorio[40];
    montarNomeArquivo(nomeArquivoAleatorio, numeroRegistros, 3);

    arquivoAleatorio = fopen(nomeArquivoAleatorio, "rb");

    if (arquivoAleatorio == NULL) {
        gerarArquivoAleatorio(numeroRegistros);
        arquivoAleatorio = fopen(nomeArquivoAleatorio, "rb");
    }

    TRegistro *registros = malloc(numeroRegistros * sizeof(TRegistro));
    // TRegistro registros[numeroRegistros];

    fread(registros, sizeof(TRegistro), numeroRegistros, arquivoAleatorio);
    fclose(arquivoAleatorio);

    // Ordena ascendentemente
    ShellSort(registros, numeroRegistros, tipoOrdenacao);

    static char nomeArquivo[40];
    montarNomeArquivo(nomeArquivo, numeroRegistros, tipoOrdenacao);

    FILE *arquivo;
    arquivo = fopen(nomeArquivo, "wb");  // Cria um arquivo binário para gravação
    fwrite(registros, sizeof(TRegistro), numeroRegistros, arquivo);

    fclose(arquivo);
    free(registros);

    return nomeArquivo;
}

char *gerarArquivoAscendente(int numeroRegistros) {
    char *nomeArquivo;
    nomeArquivo = gerarArquivoOrdenado(numeroRegistros, 1);

    return nomeArquivo;
}

char *gerarArquivoDescendente(int numeroRegistros) {
    char *nomeArquivo;
    nomeArquivo = gerarArquivoOrdenado(numeroRegistros, 2);

    return nomeArquivo;
}

char *gerarArquivoAleatorio(int numeroRegistros) {
    static char nomeArquivo[40];
    montarNomeArquivo(nomeArquivo, numeroRegistros, 3);

    /** Criando Arquivo
     * 		geração randomica de´números de 1 a 1000
     */
    FILE *arquivo;
    arquivo = fopen(nomeArquivo, "rb");

    // Permite que  caso já exista um binario aleatorio de mesmo tamnho já criado, não seja criado outro
    if (arquivo == NULL) {
        arquivo = fopen(nomeArquivo, "wb");  // Cria um arquivo binário para gravação
    } else
        return nomeArquivo;

    if (arquivo == NULL) {
        printf("Falha na geracao do arquivo externo\n");
        return "0";
    }

    // Randomizando chaves
    TRegistro *registros = malloc(numeroRegistros * sizeof(TRegistro));  // Para que seja possivel gerar mais elementos
    // TRegistro registros[numeroRegistros];

    srand(time(NULL));
    for (int i = 0; i < numeroRegistros; i++) {
        registros[i].chave = getRandomNumber();
        registros[i].dado1 = rand();

        char iString[10];
        itoa(i, iString, 10);  // Converte int para string
        char txt[] = "Registro numero ";
        strcat(txt, iString);
        strcpy(registros[i].dado2, txt);
    }

    fwrite(registros, sizeof(TRegistro), numeroRegistros, arquivo);

    fclose(arquivo);
    free(registros);

    return nomeArquivo;
}

long getRandomNumber() {
    unsigned long x;
    x = rand();
    x <<= 15;
    x ^= rand();
    x %= 1000001;

    return x;
}