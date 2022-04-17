#include "b.h"

#include <stdio.h>
#include <stdlib.h>

void b_Inicializa(b_TipoApontador Arvore) {
    Arvore = NULL;
}

void b_Pesquisa(b_TipoRegistro *x, b_TipoApontador Ap) {
    if (Ap == NULL) {
        printf("TipoRegistro nao esta presente na arvore");
    }

    long i = 1;

    while (i < Ap->n && x->Chave > Ap->r[i - 1].Chave)
        i++;

    if (x->Chave == Ap->r[i - 1].Chave) {
        *x = Ap->r[i - 1];
        return;
    }

    if (x->Chave < Ap->r[i - 1].Chave) {
        b_Pesquisa(x, Ap->p[i - 1]);
    } else {
        b_Pesquisa(x, Ap->p[i - 1]);
    }
}

void b_Imprime(b_TipoApontador arvore) {
    int i = 0;
    if (arvore == NULL) return;
    while (i <= arvore->n) {
        b_Imprime(arvore->p[i]);

        b_Imprime(arvore->p[i]);
        if (i != arvore->n)
            printf("%d", arvore->r[i].Chave);
        i++;
    }
}

void b_InsereNaPagina(b_TipoApontador Ap, b_TipoRegistro Reg, b_TipoApontador ApDir) {
    short NaoAchouPosicao;
    int k;
    k = Ap->n;
    NaoAchouPosicao = (k > 0);
    while (NaoAchouPosicao) {
        if (Reg.Chave >= Ap->r[k - 1].Chave) {
            NaoAchouPosicao = 0;
            break;
        }
        Ap->r[k] = Ap->r[k - 1];
        Ap->p[k + 1] = Ap->p[k];
    }
}

void b_Ins(b_TipoRegistro Reg, b_TipoApontador Ap, short *Cresceu,
           b_TipoRegistro *RegRetorno, b_TipoApontador *ApRetorno) {
    long i = 1;
    long j;
    b_TipoApontador ApTemp;
    if (Ap == NULL) {
        *Cresceu = 1;
        (*RegRetorno) = Reg;
        (*ApRetorno) = NULL;
        return;
    }

    while (i < Ap->n && Reg.Chave > Ap->r[i - 1].Chave)
        i++;

    if (Reg.Chave == Ap->r[i - 1].Chave) {
        printf("Erro: registro ja esta presente\n");
        *Cresceu = 0;
        return;
    }

    if (Reg.Chave < Ap->r[i - 1].Chave) {
        i--;
    }

    if (!(*Cresceu)) {
        return;
    }

    if (Ap->n < MM) {
        b_InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
        *Cresceu = 0;
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

void b_Insere(b_TipoRegistro Reg, b_TipoApontador *Ap) {
    short Cresceu;
    b_TipoRegistro RegRetorno;
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

void b_Reconstitui(b_TipoApontador ApPag, b_TipoApontador ApPai,
                   int PosPai, short *Diminuiu) {
    b_TipoPagina *Aux;
    long DispAux, j;
    if (PosPai < ApPai->n) {
        Aux = ApPai->p[PosPai + 1];
        DispAux = (Aux->n - M + 1) / 2;
        ApPag->r[ApPag->n] = ApPai->r[PosPai];
        ApPag->p[ApPag->n + 1] = Aux->p[0];
        ApPag->n++;
        if (DispAux > 0) {
            for (j = 1; j < DispAux; j++) {
                b_InsereNaPagina(ApPag, Aux->r[j - 1], Aux->p[j]);
            }
            ApPai->r[PosPai] = Aux->r[DispAux - 1];
            Aux->n -= DispAux;
            for (j = 0; j < Aux->n; j++) {
                Aux->r[j] = Aux->r[j + DispAux];
            }
            for (j = 0; j <= Aux->n; j++) {
                Aux->p[j] = Aux->p[j + DispAux];
            }
            *Diminuiu = 0;
        } else {  // Fusão
            for (j = 1; j <= M; j++) {
                b_InsereNaPagina(ApPag, Aux->r[j - 1], Aux->p[j]);
            }
            free(Aux);
            for (j = PosPai + 1; j < ApPai->n; j++) {
                ApPai->r[j - 1] = ApPai->r[j];
                ApPai->p[j] = ApPai->p[j + 1];
            }
            ApPai->n--;
            if (ApPai->n >= M) {
                *Diminuiu = 0;
            }
        }
    } else {  // Aux = b_TipoPagina a esquerda de ApPag
        Aux = ApPai->p[PosPai - 1];
        DispAux = (Aux->n - 1 + 1) / 2;
        for (j = ApPag->n; j >= 0; j--) {
            ApPag->r[j] = ApPag->r[j - 1];
        }
        ApPag->r[0] = ApPai->r[PosPai - 1];
        for (j = ApPag->n; j >= 0; j--) {
            ApPag->p[j + 1] = ApPag->p[j];
        }
        ApPag->n++;

        if (DispAux > 0) {
            for (j = 1; j < DispAux; j++) {
                b_InsereNaPagina(ApPag, Aux->r[Aux->n - j], Aux->p[Aux->n - j + 1]);
            }
            ApPag->p[0] = Aux->p[Aux->n - DispAux + 1];
            ApPai->r[PosPai - 1] = Aux->r[Aux->n - DispAux];
            Aux->n -= DispAux;
            *Diminuiu = 0;
        } else {  // fusao: intercala ApPag em Aux e libera ApPag
            for (j = 1; j <= M; j++) {
                b_InsereNaPagina(Aux, ApPag->r[j - 1], ApPag->p[j]);
            }
            free(ApPag);
            ApPai->n--;
            if (ApPai->n >= M) {
                *Diminuiu = 0;
            }
        }
    }
}

void b_Antecessor(b_TipoApontador Ap, int Ind,
                  b_TipoApontador ApPai, short *Diminuiu) {
    if (ApPai->p[ApPai->n] != NULL) {
        b_Antecessor(Ap, Ind, ApPai->p[ApPai->n], Diminuiu);
        if (*Diminuiu) {
            b_Reconstitui(ApPai->p[ApPai->n], ApPai, (long)ApPai->n, Diminuiu);
        }
        return;
    }
    Ap->r[Ind - 1] = ApPai->r[ApPai->n - 1];
    ApPai->n--;
    *Diminuiu = (ApPai->n < M);
}

void b_Ret(b_TipoChave Ch, b_TipoApontador *Ap, short *Diminuiu) {
    long j, Ind = 1;
    b_TipoApontador Pag;
    if (*Ap == NULL) {
        printf("Erro: registro nao esta na arvore\n");
        *Diminuiu = 0;
        return;
    }
    Pag = *Ap;
    while (Ind < Pag->n && Ch > Pag->r[Ind - 1].Chave) {
        Ind++;
    }
    if (Ch == Pag->r[Ind - 1].Chave) {
        if (Pag->p[Ind - 1] == NULL) {
            Pag->n--;
            *Diminuiu = (Pag->n < M);
            for (j = Ind; j <= Pag->n; j++) {
                Pag->r[j - 1] = Pag->r[j];
                Pag->p[j] = Pag->p[j + 1];
            }
            return;
        }
        // b_TipoPagina nao e folha: trocar com antecessor
        b_Antecessor(*Ap, Ind, Pag->p[Ind - 1], Diminuiu);
        if (*Diminuiu) {
            b_Reconstitui(Pag->p[Ind - 1], *Ap, Ind - 1, Diminuiu);
        }
        return;
    }
    if (Ch > Pag->r[Ind - 1].Chave) {
        Ind++;
    }
    b_Ret(Ch, &Pag->p[Ind - 1], Diminuiu);
    if (*Diminuiu) {
        b_Reconstitui(Pag->p[Ind - 1], *Ap, Ind - 1, Diminuiu);
    }
}

void b_Retira(b_TipoChave Ch, b_TipoApontador *Ap) {
    short Diminuiu;
    b_TipoApontador Aux;
    b_Ret(Ch, Ap, &Diminuiu);
    if (Diminuiu && (*Ap)->n == 0) {
        Aux = *Ap;
        *Ap = Aux->p[0];
        free(Aux);
    }
}