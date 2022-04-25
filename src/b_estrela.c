#include "b_estrela.h"


void Inicializa (TipoApontadorEstrela *Arvore)
{
    *Arvore = NULL;
}

int cont = 0;

void Pesquisa(TRegistro *x, TipoApontadorEstrela *Ap) {
    int i;
    TipoApontadorEstrela Pag;
    Pag = *Ap;

    if((*Ap)->Pt == Interna) {
        i = 1;

        while(i < Pag->UU.U0.ni && x->chave > Pag->UU.U0.ri[i - 1].chave) i++;

        if(x->chave < Pag->UU.U0.ri[i - 1].chave) {
			printf("\n\nCont: %d -- i: %d -- ni: %d -- fol: %d -- chaAt: %d\n\n", cont++, i, Pag->UU.U0.ni, Pag->UU.U1.ne, Pag->UU.U0.ri[i].chave);
            Pesquisa(x, &Pag->UU.U0.pi[i - 1]);
		}
        else
            Pesquisa(x, &Pag->UU.U0.pi[i]);

        return;        
    }

    i = 1;
    while(i < Pag->UU.U1.ne && x->chave > Pag->UU.U1.re[i - 1].chave) i++;

    if(x->chave == Pag->UU.U1.re[i - 1].chave) {
        *x = Pag->UU.U1.re[i - 1];

        printf("Chave encontrada!\n");
        printf("Chave: %d\n", x->chave);
        printf("Dado1: %lld\n", x->dado1);
        printf("Dado2: %s\n", x->dado2);
    }
    else 
        printf("TipoRegistro nao esta presente na arvore\n");
}


void InsereNaPagina(TipoApontadorEstrela Ap, TRegistro Reg, TipoApontadorEstrela ApDir) {//insere registro na pagina (folha ou indice)
    int k;
    int NaoAchouPosicao;

    if(Ap->Pt == Interna){ //se for nó interno(indice) busca na arvore onde o registro será inserido
		k = Ap->UU.U0.ni;
		NaoAchouPosicao = (k > 0);

		while(NaoAchouPosicao) {
			if (Reg.chave >= Ap->UU.U0.ri[k - 1].chave) {
				NaoAchouPosicao = 0;
				break;
			}

			Ap->UU.U0.ri[k] = Ap->UU.U0.ri[k - 1];
			Ap->UU.U0.pi[k + 1] = Ap->UU.U0.pi[k];
			k--;
			if (k < 1)
				NaoAchouPosicao = 0;
		}

		Ap->UU.U0.ri[k] = Reg;
		Ap->UU.U0.pi[k + 1] = ApDir;
		Ap->UU.U0.ni++;
		return;
		
	}
        //caso contrario, (nó externo(folha)) busca na pagina onde o registro será inserido
	k = Ap->UU.U1.ne;
	NaoAchouPosicao = (k > 0);

	while (NaoAchouPosicao)
    {
        if (Reg.chave >= Ap->UU.U1.re[k - 1].chave) 
        {
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



void Ins(TRegistro Reg, TipoApontadorEstrela Ap, int *Cresceu, TRegistro *RegRetorno, TipoApontadorEstrela *ApRetorno) {
    TipoApontadorEstrela ApTemp, Pag;
    long i = 1, j;
    Pag = Ap;
	int n;
	
	if (Ap == NULL) {
		*Cresceu = 1;
		(*RegRetorno) = Reg;
		(*ApRetorno) = NULL;
		return;
	}
	
	//caminhamento em indices
	if (Ap->Pt == Interna){
		while (i < Pag->UU.U0.ni && Reg.chave > Pag->UU.U0.ri[i - 1].chave) {
			i++;
		}

		if (Reg.chave < Pag->UU.U0.ri[i - 1].chave) {
			i--;
		}

		Ins(Reg, Pag->UU.U0.pi[i], Cresceu, RegRetorno, ApRetorno);	
		return;
	
		if (!*Cresceu) {
			return;
		}
	
	//Overflow. Página interna (indice) precisa ser dividida (excedeu limite MM)
		ApTemp = (TipoApontadorEstrela) calloc(1, sizeof(TipoPagina));
		ApTemp->UU.U0.ni = 0;
		ApTemp->UU.U0.pi[0] = NULL;

		if (i < M + 1) {
			TRegistro RegTemp;
			RegTemp.chave = Ap->UU.U0.ri[M - 1].chave;
			InsereNaPagina(ApTemp, RegTemp, Ap->UU.U0.pi[M]);
			Ap->UU.U0.ni--;
			InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
		} 
		else{
			InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
		}

		for (j = M + 2; j <= M; j++) {
			TRegistro RegTemp;
			RegTemp.chave = Ap->UU.U0.ri[j - 1].chave;
			InsereNaPagina(ApTemp, RegTemp, Ap->UU.U0.pi[j]);
		}
		Ap->UU.U0.ni = M;
		ApTemp->UU.U0.pi[0] = Ap->UU.U0.pi[M + 1];
		(*RegRetorno).chave = Ap->UU.U0.ri[M].chave;
		*ApRetorno = ApTemp;
	}

    //em paginas folha
	while (i < Pag->UU.U1.ne && Reg.chave > Pag->UU.U1.re[i - 1].chave){
			i++;
	}
	
	if (Reg.chave == Pag->UU.U1.re[i - 1].chave) {   
            //O registro ja esta presente
            //*Cresceu = 0;
            return;
        }
	
	if (Reg.chave < Ap->UU.U1.re[i - 1].chave) {
		i--;
	}

	Ins(Reg,NULL,Cresceu,RegRetorno,ApRetorno);
	if (!*Cresceu) {
		return;
	}
	
	if (Pag->UU.U1.ne < M) {
		//Pagina tem espaco 
        InsereNaPagina(Pag, *RegRetorno, *ApRetorno);
        *Cresceu = 0;
        return;
    }
		
    //Overflow. Página externa (folha) precisa ser dividida (excedeu limite MM) 
    ApTemp = (TipoApontadorEstrela) malloc(sizeof(TipoPagina));
    ApTemp->Pt = Externa;
	ApTemp->UU.U1.ne = 0;
    if (i <= M + 1)
    {
        InsereNaPagina(ApTemp, Ap->UU.U1.re[M - 1], NULL);
        Ap->UU.U1.ne--;
        InsereNaPagina(Pag, *RegRetorno, *ApRetorno);
    }
    else
        InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
    for (j = M + 2; j <= M; j++)
        InsereNaPagina(ApTemp, Pag->UU.U1.re[j - 1], Ap->UU.U0.pi[j]);
    Ap->UU.U1.ne = M;
    *RegRetorno = Ap->UU.U1.re[M];
	InsereNaPagina(ApTemp, Ap->UU.U1.re[M], NULL);
    *ApRetorno = ApTemp;
} 


//Insere registro na arvore (onde sera inserido sera verificado no Ins)
void Insere(TRegistro Reg, TipoApontadorEstrela *Ap) {
    int Cresceu;
    TRegistro RegRetorno;
    TipoPagina *ApRetorno, *ApTemp;
    
    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);

    if (Cresceu)   // Arvore cresce na altura pela raiz 
    {	
		ApTemp = (TipoPagina*) malloc (sizeof(TipoPagina));
        if(ApRetorno == NULL){
			ApTemp->Pt = Externa;
			ApTemp->UU.U1.ne = 1;
			ApTemp->UU.U1.re[0] = RegRetorno;
		}
		else{	
			ApTemp->Pt = Interna;
			ApTemp->UU.U0.ni = 1;
            ApTemp->UU.U0.ri[0] = RegRetorno; // troca
			//ApTemp->UU.U0.ri[0] = RegRetorno;
			ApTemp->UU.U0.pi[1] = ApRetorno;
			ApTemp->UU.U0.pi[0] = *Ap;
		}
		*Ap = ApTemp;
	}
}


//Inicializa arvore b estrela
void b_estrela(int quantidade, int chave, int opcional) {
    TipoApontadorEstrela Ap;
    TRegistro item;

    char *registros = gerarArquivoAleatorio(quantidade);

    FILE *arquivo = fopen(registros, "rb");

    if(opcional)
        printaRegistros(quantidade, arquivo);
 
    Ap = malloc(sizeof(TipoApontadorEstrela));
    Inicializa (&Ap);

    while (fread(&item, sizeof(TRegistro), 1, arquivo) == 1) {;
        Insere(item, &Ap);
    }

    item.chave = chave;

	// bstar_Imprime(Ap);

    // printf("\n\n%d %d %d %d %d %d\n\n", item.chave, Ap->UU.U0.ni, Ap->UU.U0.pi[0]->UU.U0.ni, Ap->UU.U0.ri[0], Ap->UU.U0.pi[0]->UU.U0.ri[0].chave, Ap->UU.U0.pi[0]->UU.U0.ri[1].chave);
    
	printf("\n AQUI ANTES %s\n", item.dado2);
    Pesquisa(&item , &Ap);
	printf("\n AQUI DEPOIS %s  ch: %d  outros: %lld\n", item.dado2, item.chave, item.dado1);
}



void bstar_Imprime(TipoApontadorEstrela arvore) {
    int i = 0;
    if (arvore == NULL || arvore->Pt == Externa) return;

    while (i <= arvore->UU.U0.ni) {
        bstar_Imprime(arvore->UU.U0.pi[i]);

        if (i != arvore->UU.U0.ni){
            printf("%d ", arvore->UU.U0.ri[i].chave);
			printf("\n\nCont: %d -- i: %d -- ni: %d -- pt: %d -- fol: %d \n\n", cont++, i, arvore->UU.U0.ni, arvore->Pt, arvore->UU.U1.ne);
		}
        i++;
    }
    
}