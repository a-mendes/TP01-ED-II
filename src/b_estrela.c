#include "b_estrela.h"

void Pesquisa(TRegistro *x, TipoApontadorEstrela *Ap) {
    int i;
    TipoApontadorEstrela Pag;
    Pag = *Ap;

    if ((*Ap)->Pt == Interna) {
        i = 1;

        while (i < Pag->UU.U0.ni && x->chave > Pag->UU.U0.ri[i - 1]) i++;

        if (x->chave < Pag->UU.U0.ri[i - 1])
            Pesquisa(x, &Pag->UU.U0.pi[i - 1]);
        else
            Pesquisa(x, &Pag->UU.U0.pi[i]);

        return;
    }

    i = 1;
    while (i < Pag->UU.U1.ne && x->chave > Pag->UU.U1.re[i - 1].chave) i++;

    if (x->chave == Pag->UU.U1.re[i - 1].chave)
        *x = Pag->UU.U1.re[i - 1];
    else
        printf("TipoRegistro nao esta presente na arvore\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void b_Inicializa(TipoApontadorEstrela Arvore) {
    Arvore = NULL;
}

void b_Pesquisa(TRegistro *x, TipoApontadorEstrela Ap) {
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

void b_Imprime(TipoApontadorEstrela arvore) {
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

void b_InsereNaPagina(TipoApontadorEstrela Ap, TRegistro Reg, TipoApontadorEstrela ApDir) {
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

void b_Ins(TRegistro Reg, TipoApontadorEstrela Ap, short *Cresceu,
           TRegistro *RegRetorno, TipoApontadorEstrela *ApRetorno) {
    long i = 1;
    long j;
    TipoApontadorEstrela ApTemp;
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

    ApTemp = (TipoApontadorEstrela)malloc(sizeof(b_TipoPagina));
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

void b_Insere(TRegistro Reg, TipoApontadorEstrela *Ap) {
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