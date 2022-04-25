#include "b.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "extra/extra.h"
#include "extra/gerador_arquivo.h"

int b(int quantidade, int situacao, int chave, int opcional) {
    char *registros = gerarArquivoOrdenado(quantidade, situacao);
    FILE *arquivo = fopen(registros, "rb");
    // Imprime os registros
    if (opcional) {
        printf("opcional");
        printaRegistros(quantidade, arquivo);
    }

    b_TipoApontador arvore = montarArvoreBFromArquivo(quantidade, arquivo);
    fclose(arquivo);

    montarArquivoFromArvoreB(arvore);

    // fecha o arquivo de registros

    if (chave != -1) {
        b_PesquisaComTimer(chave);
    } else {
        for (int i = 0; i < 20; i++) {
            b_PesquisaComTimer(getRandomNumber());
        }
    }

    return 0;
}

void imprimirNodo(b_TipoApontador nodo) {
    printf("N: %d, ", nodo->n);
    printf("Chaves: ");
    for (int i = 0; i < nodo->n; i++) {
        printf("%d ", nodo->r[i].chave);
    }
    printf("\n");
}

b_Bloco criarBlocoArvore(b_TipoApontador arvore, int father_pointer) {
    b_Bloco bloco;
    bloco.n = arvore->n;
    memcpy(&bloco.r, arvore->r, sizeof(arvore->r));
    for (int i = 0; i < MM + 1; i++) {
        bloco.p[i] = -1;
    }

    // imprimirNodo(arvore);

    return bloco;
}

void escreverNoArquivo(FILE *arquivo, b_TipoApontador arvore, int *current_pointer, int father_pointer, OpCounter *operacoes) {
    // printf("%d ", operacoes->comparisons++);
    if (arvore == NULL) {
        return;
    }

    (*current_pointer)++;

    b_Bloco bloco = criarBlocoArvore(arvore, father_pointer);
    // escreve o bloco na memoria
    fseek(arquivo, (*current_pointer) * sizeof(b_Bloco), SEEK_SET);
    fwrite(&bloco, sizeof(b_Bloco), 1, arquivo);
    operacoes->transfers++;

    // coloca o current_pointer no lugar de um apontador vazio do pai
    if (father_pointer != -1) {
        fseek(arquivo, father_pointer * sizeof(b_Bloco), SEEK_SET);
        fread(&bloco, sizeof(b_Bloco), 1, arquivo);
        operacoes->transfers++;

        int i = 0;
        while (bloco.p[i] != -1) i++;
        bloco.p[i] = (*current_pointer);

        fseek(arquivo, father_pointer * sizeof(b_Bloco), SEEK_SET);
        fwrite(&bloco, sizeof(b_Bloco), 1, arquivo);
        operacoes->transfers++;
    }

    father_pointer = *current_pointer;

    for (int i = 0; i < arvore->n + 1; i++) {
        escreverNoArquivo(arquivo, arvore->p[i], current_pointer, father_pointer, operacoes);
    }
}

void imprimirArquivoArvoreB(FILE *arquivo) {
    rewind(arquivo);
    b_Bloco bloco;
    int i = 0;
    while (fread(&bloco, sizeof(b_Bloco), 1, arquivo)) {
        printf("Bloco %d | ", i);
        printf("N: %d, ", bloco.n);
        printf("Chaves: ");
        for (int i = 0; i < bloco.n; i++) {
            printf("%d ", bloco.r[i].chave);
        }

        printf("Apontadores: ");
        for (int i = 0; i < bloco.n + 1; i++) {
            printf("%d ", bloco.p[i]);
        }
        printf("\n");
        i++;
    }
}

void montarArquivoFromArvoreB(b_TipoApontador arvore) {
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    FILE *arquivo = fopen("data/btree.dat", "wb+");

    OpCounter operacoes = (OpCounter){0};
    int count = -1;
    escreverNoArquivo(arquivo, arvore, &count, -1, &operacoes);
    // imprimirArquivoArvoreB(arquivo);

    fclose(arquivo);

    gettimeofday(&stop, NULL);
    printf("\nTempo de execucao (criacao do arquivo btree.dat): %lu us\nNumero de transferencias: %ld\n\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec, operacoes.transfers);  // imprime o tempo de execução
}

void b_PesquisaComTimer(int chave) {
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    FILE *arquivo = fopen("data/btree.dat", "rb");

    OpCounter operacoes = (OpCounter){0};
    TRegistro item = {.chave = chave, .dado1 = 0, .dado2 = 0};
    if (b_PesquisaArquivo(&item, 0, arquivo, &operacoes)) {
        printf("Chave %d presente | Dado 1: %lld, Dado2: %s", item.chave, item.dado1, item.dado2);
    } else {
        printf("Chave %d nao esta presente", chave);
    }

    fclose(arquivo);

    gettimeofday(&stop, NULL);
    printf("\nTempo de execucao (pesquisa no arquivo): %lu us\nNumero de transferencias: %ld\nNumero de comparacoes: %ld\n\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec, operacoes.transfers, operacoes.comparisons);
}

b_TipoApontador montarArvoreBFromArquivo(int quantidade, FILE *arquivo) {
    b_TipoApontador arvore;
    b_Inicializa(&arvore);

    rewind(arquivo);
    TRegistro *registros = malloc(ITENSPAGINA * sizeof(TRegistro));
    const int qntPag = quantidade < ITENSPAGINA ? 1 : quantidade / ITENSPAGINA;

    for (int i = 0; i < qntPag; ++i) {
        fread(registros, sizeof(TRegistro), ITENSPAGINA, arquivo);
        for (int j = 0; j < ITENSPAGINA; j++) {
            b_Insere(registros[j], &arvore);
        }
    }

    free(registros);
    return arvore;
}

void b_Inicializa(b_TipoApontador *Arvore) {
    (*Arvore) = NULL;
}

int b_Pesquisa(TRegistro *x, b_TipoApontador Ap) {
    if (Ap == NULL) {
        return 0;
    }

    long i = 1;

    while (i < Ap->n && x->chave > Ap->r[i - 1].chave)
        i++;

    if (x->chave == Ap->r[i - 1].chave) {
        *x = Ap->r[i - 1];
        return 1;
    }

    if (x->chave < Ap->r[i - 1].chave) {
        return b_Pesquisa(x, Ap->p[i - 1]);
    } else {
        return b_Pesquisa(x, Ap->p[i]);
    }
}

int b_PesquisaArquivo(TRegistro *x, long current_line, FILE *arquivo, OpCounter *operacoes) {
    operacoes->comparisons++;
    if (current_line == -1) {
        return 0;
    }

    b_Bloco bloco;
    fseek(arquivo, current_line * sizeof(b_Bloco), SEEK_SET);
    fread(&bloco, sizeof(b_Bloco), 1, arquivo);
    operacoes->transfers++;

    long i = 1;

    operacoes->comparisons++;
    while (i < bloco.n && x->chave > bloco.r[i - 1].chave) {
        operacoes->comparisons++;
        i++;
    }

    operacoes->comparisons++;
    if (x->chave == bloco.r[i - 1].chave) {
        *x = bloco.r[i - 1];
        return 1;
    }

    operacoes->comparisons++;
    if (x->chave < bloco.r[i - 1].chave) {
        return b_PesquisaArquivo(x, bloco.p[i - 1], arquivo, operacoes);
    } else {
        return b_PesquisaArquivo(x, bloco.p[i], arquivo, operacoes);
    }
}

void b_Imprime(b_TipoApontador arvore) {
    int i = 0;
    if (arvore == NULL) return;
    while (i <= arvore->n) {
        b_Imprime(arvore->p[i]);

        if (i != arvore->n)
            printf("%d ", arvore->r[i].chave);
        i++;
    }
}

void b_Insere(TRegistro Reg, b_TipoApontador *Ap) {
    short Cresceu;
    TRegistro RegRetorno;
    b_TipoPagina *ApRetorno, *ApTemp;

    b_Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);

    if (Cresceu) {
        ApTemp = (b_TipoPagina *)malloc(sizeof(b_TipoPagina));
        ApTemp->n = 1;
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *Ap;
        *Ap = ApTemp;
    }
}

void b_Ins(TRegistro Reg, b_TipoApontador Ap, short *Cresceu,
           TRegistro *RegRetorno, b_TipoApontador *ApRetorno) {
    long i = 1, j;
    b_TipoApontador ApTemp;
    if (Ap == NULL) {
        *Cresceu = 1;
        (*RegRetorno) = Reg;
        (*ApRetorno) = NULL;
        return;
    }

    while (i < Ap->n && Reg.chave > Ap->r[i - 1].chave)
        i++;

    if (Reg.chave == Ap->r[i - 1].chave) {
        *Cresceu = 0;
        return;
    }

    if (Reg.chave < Ap->r[i - 1].chave) {
        i--;
    }

    b_Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);

    if (!(*Cresceu)) {
        return;
    }

    if (Ap->n < MM) {
        b_InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
        *Cresceu = 0;
        return;
    }

    ApTemp = (b_TipoApontador)malloc(sizeof(b_TipoPagina));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;

    if (i < M + 1) {
        b_InsereNaPagina(ApTemp, Ap->r[MM - 1], Ap->p[MM]);
        Ap->n--;
        b_InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    } else {
        b_InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
    }

    for (j = M + 2; j <= MM; j++) {
        b_InsereNaPagina(ApTemp, Ap->r[j - 1], Ap->p[j]);
    }

    Ap->n = M;
    ApTemp->p[0] = Ap->p[M + 1];
    *RegRetorno = Ap->r[M];
    *ApRetorno = ApTemp;
}

void b_InsereNaPagina(b_TipoApontador Ap, TRegistro Reg, b_TipoApontador ApDir) {
    short NaoAchouPosicao;
    int k;
    k = Ap->n;
    NaoAchouPosicao = (k > 0);
    while (NaoAchouPosicao) {
        if (Reg.chave >= Ap->r[k - 1].chave) {
            NaoAchouPosicao = 0;
            break;
        }
        Ap->r[k] = Ap->r[k - 1];
        Ap->p[k + 1] = Ap->p[k];
        k--;
        if (k < 1) {
            NaoAchouPosicao = 0;
        }
    }
    Ap->r[k] = Reg;
    Ap->p[k + 1] = ApDir;
    Ap->n++;
}