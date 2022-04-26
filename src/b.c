#include "b.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "extra/extra.h"
#include "extra/gerador_arquivo.h"

int b(int quantidade, int situacao, int chave, int opcional) {
    // Gera o arquivo de acordo com a situação especificada
    char *registros = gerarArquivoOrdenado(quantidade, situacao);
    FILE *arquivo = fopen(registros, "rb");

    // Cria uma árvore na memória com os valores do arquivo
    b_TipoApontador arvore = montarArvoreBFromArquivo(quantidade, arquivo);

    // Cria um arquivo btree.dat com a árvore da memória
    montarArquivoFromArvoreB(arvore, opcional);

    b_Free(arvore);

    // Realiza a pesquisa no arquivo
    // caso não seja especificada uma chave, faz uma pesquisa com 20 chaves aleatórias
    if (chave != -1) {
        b_PesquisaComTimer(chave);
    } else {
        int chaves[20];
        obter20RegistrosAleatorios(arquivo, quantidade, chaves);
        for (int i = 0; i < 20; i++) {
            b_PesquisaComTimer(chaves[i]);
        }
    }

    fclose(arquivo);
    return 0;
}

void b_Free(b_TipoApontador arvore) {
    if (arvore == NULL) {
        return;
    }

    for (int i = 0; i < arvore->n + 1; i++) {
        b_Free(arvore->p[i]);
    }

    free(arvore);
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
    for (int i = 0; i < b_MM + 1; i++) {
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

    (*current_pointer)++;  // incrementa o valor da linha atual

    // Instancia um novo bloco (página) com os valores do nodo atual
    b_Bloco bloco = criarBlocoArvore(arvore, father_pointer);

    // Escreve o bloco na linha atual do arquivo
    fseek(arquivo, (*current_pointer) * sizeof(b_Bloco), SEEK_SET);
    fwrite(&bloco, sizeof(b_Bloco), 1, arquivo);
    operacoes->transfers++;

    // retorna ao bloco pai para preencher o apontador do filho
    if (father_pointer != -1) {
        // lê o nodo pai
        fseek(arquivo, father_pointer * sizeof(b_Bloco), SEEK_SET);
        fread(&bloco, sizeof(b_Bloco), 1, arquivo);
        operacoes->transfers++;

        // preenche um apontador -1 do bloco pai com o valor do current_pointer
        int i = 0;
        while (bloco.p[i] != -1) i++;
        bloco.p[i] = (*current_pointer);

        // reescreve o bloco pai
        fseek(arquivo, father_pointer * sizeof(b_Bloco), SEEK_SET);
        fwrite(&bloco, sizeof(b_Bloco), 1, arquivo);
        operacoes->transfers++;
    }

    // atribui o valor do ponteiro atual no ponteiro pai para as chamadas recursivas nos blocos filhos
    father_pointer = *current_pointer;

    // chamada recursiva em cada bloco filho
    for (int i = 0; i < arvore->n + 1; i++) {
        escreverNoArquivo(arquivo, arvore->p[i], current_pointer, father_pointer, operacoes);
    }
}

void imprimirArquivoArvoreB(FILE *arquivo) {
    rewind(arquivo);
    b_Bloco bloco;
    int i = 0;
    while (fread(&bloco, sizeof(b_Bloco), 1, arquivo)) {
        printf("Bloco %d ", i);
        // printf("N: %d, ", bloco.n);
        printf("\nChaves: ");
        for (int i = 0; i < bloco.n; i++) {
            printf("%d ", bloco.r[i].chave);
        }

        printf("\nApontadores: ");
        for (int i = 0; i < bloco.n + 1; i++) {
            printf("%d ", bloco.p[i]);
        }
        printf("\n---------------------------------------------\n");
        i++;
    }
}

void montarArquivoFromArvoreB(b_TipoApontador arvore, int opcional) {
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    FILE *arquivo = fopen("data/btree.dat", "wb+");

    OpCounter operacoes = (OpCounter){0};
    int count = -1;

    // Função recursiva para realizar a escrita no arquivo
    escreverNoArquivo(arquivo, arvore, &count, -1, &operacoes);

    // imprimir os índices
    if (opcional) {
        imprimirArquivoArvoreB(arquivo);
    }

    fclose(arquivo);

    gettimeofday(&stop, NULL);
    printf("\nTempo de execucao (criacao do arquivo btree.dat): %lu us\nNumero de transferencias: %ld\n\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec, operacoes.transfers);  // imprime o tempo de execução
}

// Pesquisa no arquivo btree.dat
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

// Cria uma árvore na memória com os dados do arquivo
b_TipoApontador montarArvoreBFromArquivo(int quantidade, FILE *arquivo) {
    // Inicializa a arvore
    b_TipoApontador arvore;
    b_Inicializa(&arvore);

    rewind(arquivo);
    TRegistro *registros = malloc(ITENSPAGINA * sizeof(TRegistro));
    const int qntPag = quantidade < ITENSPAGINA ? 1 : quantidade / ITENSPAGINA;

    // Lê o arquivo em intervalos definidos por ITENSPAGINA
    for (int i = 0; i < qntPag; i++) {
        fread(registros, sizeof(TRegistro), ITENSPAGINA, arquivo);
        for (int j = 0; j < ITENSPAGINA; j++) {
            // Insere cada registro na árvore
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
    // caso chegue em um apontador vazio (-1), o item nao foi encontrado
    operacoes->comparisons++;
    if (current_line == -1) {
        return 0;
    }

    // faz a leitura do bloco localizado na linha atual da chamada recursiva
    b_Bloco bloco;
    fseek(arquivo, current_line * sizeof(b_Bloco), SEEK_SET);
    fread(&bloco, sizeof(b_Bloco), 1, arquivo);
    operacoes->transfers++;

    long i = 1;

    // busca o índice com a chave mais proxima de x
    operacoes->comparisons++;
    while (i < bloco.n && x->chave > bloco.r[i - 1].chave) {
        operacoes->comparisons += 2;
        i++;
    }

    // caso tenha a mesma chave de x, encontrou o registro
    operacoes->comparisons++;
    if (x->chave == bloco.r[i - 1].chave) {
        *x = bloco.r[i - 1];
        return 1;
    }

    // faz a chamada recursiva com o current_pointer do filho
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

    if (Ap->n < b_MM) {
        b_InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
        *Cresceu = 0;
        return;
    }

    ApTemp = (b_TipoApontador)malloc(sizeof(b_TipoPagina));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;

    if (i < b_M + 1) {
        b_InsereNaPagina(ApTemp, Ap->r[b_MM - 1], Ap->p[b_MM]);
        Ap->n--;
        b_InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    } else {
        b_InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
    }

    for (j = b_M + 2; j <= b_MM; j++) {
        b_InsereNaPagina(ApTemp, Ap->r[j - 1], Ap->p[j]);
    }

    Ap->n = b_M;
    ApTemp->p[0] = Ap->p[b_M + 1];
    *RegRetorno = Ap->r[b_M];
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