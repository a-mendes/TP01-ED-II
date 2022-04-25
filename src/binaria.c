#include "binaria.h"

//inicia a pesquisa
int binaria(int quantidade, int situacao, int chave, int opcional){
    Noh *t; //Arvore Binaria
    TRegistro *aux;  // item para leitura de registros
    double tamanho;  // tamanho da tabela de índices

    // variáveis para medir o tempo de execução
    clock_t startIndice, endIndice;
    double time;
    
    inicia(t);

    // Gera o arquivo com a quantidade de registros informada
    char *registros = gerarArquivoAscendente(quantidade);

    // Abre o arquivo de registros
    FILE *arquivo = fopen(registros, "rb");

    //impressao de itens
    if (opcional) {
        printf("opcional");
        printaRegistros(quantidade, arquivo);
    }

    //alocando memoria pra variavel de registros
    aux = (TRegistro *)malloc(quantidade * sizeof(TRegistro));

    startIndice = clock();  // inicia a contagem do tempo

    //lendo os registros do livro
    fread(aux, sizeof(TRegistro), quantidade, arquivo);

    for(int i = 0; i < quantidade; i++)
        insere(t, aux[i]);  // salva a chave na arvore
    
    endIndice = clock();                                            // finaliza o cronômetro

    time = ((double)(endIndice - startIndice)) / CLOCKS_PER_SEC;    // calcula o tempo de execução
    printf("Tempo de execucao (criacao de indices): %lf\n", time);  // imprime o tempo de execução

    // função de busca
    if (pesquisar(t, chave) != NULL) {
        printf("Registro encontrado!\n");
        printf("Chave: %d\n", aux->chave);
        printf("Valor: %ld\n", aux->dado1);
        printf("Nome: %s\n", aux->dado2);
    } else {
        printf("Registro nao encontrado!\n");
        printf("Chave: %d\n", chave);
    }
    
    free(t);      // libera a arvore
    free(aux);    // libera o auxiliar

    fclose(arquivo);  // fecha o arquivo de registros

    return 0;
}

//inicia a arvore
void inicia(Noh *raiz)
{   
    raiz = NULL;
}

//pesquisar percorrendo a árvore
Noh* pesquisar(Noh *raiz, int x)
{
    if(raiz == NULL){ //caso seja nulo
        printf("null ");
        return NULL;
    }
    if(raiz->n.chave == x){ //caso encontre
        printf("igual ");
        return raiz;
    }
    else if(x < raiz->n.chave){ // caso seja menor que a pesquisa, percorre recursivamente pra esquerda 
        printf("menor ");
        return pesquisar(raiz->left, x);
    }
    else{ // caso seja maior que a pesquisa, percorre recursivamente pra direita 
        printf("maior ");
        return pesquisar(raiz->right, x);
    }
}

//inicializa um nó
Noh* criarNoh(TRegistro x)
{
    Noh* no = (Noh*) malloc(sizeof(Noh));
    
    no->n = x; //inserindo o valor x
    no->left = NULL; //inicializando o filho esquerdo como nulo
    no->right = NULL; //inicializando o filho direito como nulo
    
    return no;
}

void insere(Noh *raiz, TRegistro x)
{
    if(raiz == NULL){
        printf("nulo ");
        raiz = criarNoh(x);
    }
    else if(x.chave > raiz->n.chave){ 
        printf("> ");
        //raiz->right = 
        insere(raiz->right, x);
    }
    else if(x.chave < raiz->n.chave){
        printf("< ");
        //raiz->left = 
        insere(raiz->left, x);
    }

    //return raiz;//caso ja exista
}