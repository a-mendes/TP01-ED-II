#include "b_estrela.h"

void Inicializa(ApontadorEstrela *Arvore) {
    *Arvore = NULL;
}

void Pesquisa(TRegistro *registro, ApontadorEstrela *Ap) {
    // pesquisa pelo item desejado - aponta para saber se é interno ou externo
    int i;
    ApontadorEstrela pagina;
    pagina = *Ap;

    if ((*Ap)->NosOuFolha == Interna) {
        i = 1;

        while (i < pagina->ESTRUTURA.NOS.numeroChaves && registro->chave > pagina->ESTRUTURA.NOS.chave[i - 1]) i++;

        if (registro->chave < pagina->ESTRUTURA.NOS.chave[i - 1])
            Pesquisa(registro, &pagina->ESTRUTURA.NOS.ponteiroTree[i - 1]);
        else
            Pesquisa(registro, &pagina->ESTRUTURA.NOS.ponteiroTree[i]);

        return;
    }

    i = 1;
    while (i < pagina->ESTRUTURA.FOLHAS.numeroRegistros && registro->chave > pagina->ESTRUTURA.FOLHAS.registro[i - 1].chave) i++;

    if (registro->chave == pagina->ESTRUTURA.FOLHAS.registro[i - 1].chave)
        *registro = pagina->ESTRUTURA.FOLHAS.registro[i - 1];
    else
        printf("TipoRegistro nao esta presente na arvore\n");
}

void InsereNaPagina(ApontadorEstrela Ap, TRegistro registro, ApontadorEstrela ApDir) {
    short NaoAchouPosicao;
    int k;

    if (Ap->NosOuFolha == Interna) {
        k = Ap->ESTRUTURA.NOS.numeroChaves;
        NaoAchouPosicao = (k > 0);

        while (NaoAchouPosicao) {
            if (registro.chave >= Ap->ESTRUTURA.NOS.chave[k - 1]) {
                NaoAchouPosicao = 0;
                break;
            }
            Ap->ESTRUTURA.NOS.chave[k] = Ap->ESTRUTURA.NOS.chave[k - 1];
            Ap->ESTRUTURA.NOS.ponteiroTree[k + 1] = Ap->ESTRUTURA.NOS.ponteiroTree[k];
            k--;
            if (k < 1)
                NaoAchouPosicao = 0;
        }

        Ap->ESTRUTURA.NOS.chave[k] = registro;
        Ap->ESTRUTURA.NOS.ponteiroTree[k + 1] = ApDir;
        Ap->ESTRUTURA.NOS.numeroChaves++;
        return;
    }

    k = Ap->ESTRUTURA.FOLHAS.numeroRegistros;
    NaoAchouPosicao = (k > 0);

    while (NaoAchouPosicao) {
        if (registro.chave >= Ap->ESTRUTURA.FOLHAS.registro[k - 1].chave) {
            NaoAchouPosicao = 0;
            break;
        }

        Ap->UU.U1.re[k] = Ap->UU.U1.re[k - 1];
        k--;
        if (k < 1)
            NaoAchouPosicao = 0;
    }
    Ap->UU.U1.re[k] = Reg;
    Ap->UU.U1.ne++;
}

void b_Ins(TRegistro Reg, ApontadorEstrela Ap, short *Cresceu,
           TRegistro *RegRetorno, ApontadorEstrela *ApRetorno) {
    long i = 1;
    long j;
    ApontadorEstrela ApTemp;
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

    ApTemp = (ApontadorEstrela)malloc(sizeof(b_TipoPagina));
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

void b_Insere(TRegistro Reg, ApontadorEstrela *Ap) {
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