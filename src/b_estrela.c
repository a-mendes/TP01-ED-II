#include "b_estrela.h"

void bstar_Inicializa(TipoApontadorEstrela *Arvore) {
    Arvore = NULL;
}

void bstar_Pesquisa(TRegistro *x, TipoApontadorEstrela *Ap, int key, int *nTransfer, int *nCompare) {
    if ((*Ap) == NULL) {
        printf("TipoRegistro nao esta presente na arvore\n");
        return;
    }

    int i;
    TipoApontadorEstrela Pag;
    (*Ap) = *Ap;

    printf("%s\n", (*Ap)->Pt == Interna ? "Interna" : "Externa");

    if ((*Ap)->Pt == Interna) {
        i = 1;
        while (i < (*Ap)->UU.U0.ni && key > (*Ap)->UU.U0.ri[i - 1].chave) i++;
        printf("Chave procurada: %d\n", key);
        printf("Chave atual: %d\n", (*Ap)->UU.U0.ri[i - 1].chave);
        printf("Chave à direita: %d\n", (*Ap)->UU.U0.ri[i].chave);
        if (key < (*Ap)->UU.U0.ri[i - 1].chave)
            bstar_Pesquisa(x, &(*Ap)->UU.U0.pi[i - 1], key, nTransfer, nCompare);
        else
            bstar_Pesquisa(x, &(*Ap)->UU.U0.pi[i], key, nTransfer, nCompare);

        return;
    }

    i = 1;

    while (i < (*Ap)->UU.U1.ne && key > (*Ap)->UU.U1.re[i - 1].chave) i++;

    printf("Chave que existe: %d\n", (*Ap)->UU.U1.re[i - 1].chave);

    if (key == (*Ap)->UU.U1.re[i - 1].chave) {
        *x = (*Ap)->UU.U1.re[i - 1];

        printf("Chave encontrada!\n");
        printf("Chave: %d\n", x->chave);
        printf("Dado1: %lld\n", x->dado1);
        printf("Dado2: %s\n", x->dado2);
	}
    else
        printf("TipoRegistro nao esta presente na arvore\n");
}

void bstar_LerArquivo(FILE *file, int amount, TipoApontadorEstrela *Arvore) {
    TRegistro reg;
    int cont = 0;
    while ((fread(&reg, sizeof(reg), 1, file) == 1) && cont < amount) {
        bstar_Insere(reg, Arvore);
        // escreverValor(Arvore);
        cont++;
    }
}

void bstar_Ins(TRegistro reg, TipoApontadorEstrela Ap, short *Cresceu, TRegistro *RegRetorno, TipoApontadorEstrela *ApRetorno) {
    long i = 1;
    long j;
    TipoApontadorEstrela ApTemp;

    if (Ap == NULL) {
        *Cresceu = 1;
        (*RegRetorno) = reg;
        (*ApRetorno) = NULL;
        return;
    }

    while (i < Ap->UU.U0.ni && reg.chave > Ap->UU.U0.ri[i - 1].chave) i++;

    // Isso não vai acontecer por causa do conteúdo ser gerado sem repetição
    if (reg.chave == Ap->UU.U0.ri[i - 1].chave) {
        // printf("ERRO: Registro já existente\n");
        // teste // bstar_InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
        *Cresceu = 0;
        Ap->Pt = Interna;
        return;
    }

    if (reg.chave < Ap->UU.U0.ri[i - 1].chave) i--;
	
    bstar_Ins(reg, Ap->UU.U0.pi[i], Cresceu, RegRetorno, ApRetorno);

    if (!*Cresceu) 
		return;

    if (Ap->UU.U0.ni < MM) { /* Página tem Espaço */
        bstar_InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
        printf("%s\n", Ap->Pt == Interna ? "Interna" : "Externa");
        *Cresceu = 0;
        return;
    }

    /* Overflow: Página tem que ser dividida */
    ApTemp = malloc(sizeof(TipoPagina));
    ApTemp->UU.U0.ni = 0;
    ApTemp->UU.U0.pi[0] = NULL;

    if (i < M + 1) {
        bstar_InsereNaPagina(ApTemp, Ap->UU.U0.ri[MM - 1], Ap->UU.U0.pi[MM]);
        printf("Registro %d adicionada na página dividida.\n", RegRetorno->chave);
        Ap->UU.U0.ni--;
        bstar_InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    } else
        bstar_InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);

    for (j = M + 2; j <= MM; j++) 
		bstar_InsereNaPagina(ApTemp, Ap->UU.U0.ri[j - 1], Ap->UU.U0.pi[j]);

    Ap->UU.U1.ne = M;
    ApTemp->UU.U0.pi[0] = Ap->UU.U0.pi[M + 1];
    *RegRetorno = Ap->UU.U0.ri[M];
    *ApRetorno = ApTemp;
}

void bstar_Insere(TRegistro reg, TipoApontadorEstrela *Ap) {
    short Cresceu;
    TRegistro RegRetorno;
    TipoPagina *ApRetorno, *ApTemp;
    bstar_Ins(reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);

    if (Cresceu) { /* Arvore cresce na altura pela raiz */
        ApTemp = (TipoPagina *)malloc(sizeof(TipoPagina));
        ApTemp->UU.U1.ne = 1;
        ApTemp->UU.U1.re[0] = RegRetorno;
        ApTemp->UU.U0.pi[1] = ApRetorno;
        ApTemp->UU.U0.pi[0] = *Ap;
        *Ap = ApTemp;
    }
}

void bstar_InsereNaPagina(TipoApontadorEstrela Ap, TRegistro Reg, TipoApontadorEstrela ApDir) {
    int k = Ap->UU.U1.ne;
    short NaoAchouPosicao = (k > 0);

    while (NaoAchouPosicao) {
        if (Reg.chave >= Ap->UU.U1.re[k - 1].chave) {
            NaoAchouPosicao = 0;
            break;
        }
        Ap->UU.U1.re[k] = Ap->UU.U1.re[k - 1];
        Ap->UU.U0.pi[k + 1] = Ap->UU.U0.pi[k];
        k--;
        if (k < 1) NaoAchouPosicao = 0;
    }

    Ap->UU.U1.re[k] = Reg;
    Ap->UU.U0.pi[k + 1] = ApDir;
    Ap->UU.U1.ne++;
    // Ap->PT=Externa;
    escreverValor(&Ap);
}

void escreverValor(TipoApontadorEstrela *Ap) {
    printf("PT: %s", (*Ap)->Pt == Interna ? "Interna" : "Externa");
    if ((*Ap)->Pt == Interna) {
        for (int i = 0; i < (*Ap)->UU.U0.ni; ++i) {
            printf("Codigo: %d\n", (*Ap)->UU.U0.ri[i].chave);
        }
    } else {
        for (int i = 0; i < (*Ap)->UU.U1.ne; ++i) {
            printf("Codigo: %d\n", (*Ap)->UU.U1.re[i].chave);
        }
    }
}


TipoApontadorEstrela montarArvoreBEdoArquivo(int quantidade, FILE *arquivo) {
    // Inicializa a arvore
    TipoApontadorEstrela arvore;
    bstar_Inicializa(&arvore);

    rewind(arquivo);
    TRegistro *registros = malloc(ITENSPAGINA * sizeof(TRegistro));
    const int qntPag = quantidade < ITENSPAGINA ? 1 : quantidade / ITENSPAGINA;

    // Lê o arquivo em intervalos definidos por ITENSPAGINA
    for (int i = 0; i < qntPag; i++) {
        fread(registros, sizeof(TRegistro), ITENSPAGINA, arquivo);
        for (int j = 0; j < ITENSPAGINA; j++) {
            // Insere cada registro na árvore
            bstar_Insere(registros[j], &arvore);
        }
    }

    free(registros);
    return arvore;
}


void bstar(int quantidade, int situacao, int chave, int opcional) {
    TRegistro item;

    char *registros = gerarArquivoOrdenado(quantidade, situacao);
    FILE *arquivo = fopen(registros, "rb");

	// TipoApontadorEstrela arvore = montarArvoreBEdoArquivo(quantidade, arquivo);

    if(opcional)
        printaRegistros(quantidade, arquivo);

    TipoApontadorEstrela Ap;
    bstar_Inicializa(&Ap);

    while (fread(&item, sizeof(TRegistro), 1, arquivo) == 1) {
		printf("ITEM %d %lld %s \n\n", item.chave, item.dado1, item.dado2);
        bstar_Insere(item, &Ap);
    }

    item.chave = chave;
	int o,j;
	// bstar_Imprime(Ap);

    // printf("\n\n%d %d %d %d %d %d\n\n", item.chave, Ap->UU.U0.ni, Ap->UU.U0.pi[0]->UU.U0.ni, Ap->UU.U0.ri[0], Ap->UU.U0.pi[0]->UU.U0.ri[0].chave, Ap->UU.U0.pi[0]->UU.U0.ri[1].chave);
    
	printf("\n AQUI ANTES %s\n", item.dado2);
    bstar_Pesquisa(&item , &Ap, chave, &o, &j);
	printf("\n AQUI DEPOIS %s  ch: %d  outros: %lld\n", item.dado2, item.chave, item.dado1);
}

void bstar_teste() {
    TipoApontadorEstrela zap;
    bstar_Inicializa(&zap);

	TRegistro nd[4];
	nd[0].chave = 50;
	nd[0].dado1 = 5848950;
	nd[0].dado2[0] = 'o';

	nd[1].chave = 150;
	nd[1].dado1 = 545150;
	nd[1].dado2[0] = 'p';

	nd[2].chave = 250;
	nd[2].dado1 = 415250;
	nd[2].dado2[0] = 'i';

	nd[3].chave = 350;

	nd[4].chave = 450;

	nd[5].chave = 550;

	printf("\nOI\n");
    bstar_Insere(nd[0], &zap);
    bstar_Insere(nd[1], &zap);
    bstar_Insere(nd[2], &zap);
    bstar_Insere(nd[3], &zap);
    bstar_Insere(nd[4], &zap);
    bstar_Insere(nd[5], &zap);
	printf("\nOI\n");
    TRegistro a;
    a.chave = 1;
    int az;
    bstar_Pesquisa(&a, &zap, 25, &az, &az);
}