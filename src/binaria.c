#include "binaria.h"

//inicia a pesquisa
int binaria(int quantidade, int situacao, int chave, int opcional){
    Noh *t; //Arvore Binaria
    TRegistro *aux;  // item para leitura de registros
    double tamanho;  // tamanho da tabela de índices

    // variáveis para medir o tempo de execução
    clock_t startIndice, endIndice;
    double time;
    
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

    t = criarNoh(aux[0]);

    for(int i = 0; i < quantidade; i++)
        insere(t, aux[i+1]);  // salva a chave na arvore
    
    endIndice = clock();                                            // finaliza o cronômetro

    time = ((double)(endIndice - startIndice)) / CLOCKS_PER_SEC;    // calcula o tempo de execução
    printf("Tempo de execucao (criacao de indices): %lf\n", time);  // imprime o tempo de execução

    // função de busca
    Noh* pesquisa = pesquisar(t, chave);
    if (pesquisa != NULL) {
        printf("Registro encontrado!\n");
        printf("Chave: %d\n", pesquisa->n.chave);
        printf("Valor: %ld\n", pesquisa->n.dado1);
        printf("Nome: %s\n", pesquisa->n.dado2);
    } else {
        printf("Registro nao encontrado!\n");
        printf("Chave: %d\n", chave);
    }
    
    free(t);      // libera a arvore
    free(aux);    // libera o auxiliar

    fclose(arquivo);  // fecha o arquivo de registros

    return 0;
}

//pesquisar percorrendo a árvore
Noh* pesquisar(Noh *raiz, int x)
{
    if(raiz == NULL){ //caso seja nulo
        return NULL;
    }
    if(raiz->n.chave == x){ //caso encontre
        return raiz;
    }
    else if(x < raiz->n.chave){ // caso seja menor que a pesquisa, percorre recursivamente pra esquerda 
        return pesquisar(raiz->left, x);
    }
    else{ // caso seja maior que a pesquisa, percorre recursivamente pra direita 
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

Noh* insere(Noh *raiz, TRegistro x)
{
    if(raiz == NULL){
        raiz = criarNoh(x);
    }
    else if(x.chave > raiz->n.chave){ 
        raiz->right = insere(raiz->right, x);
    }
    else if(x.chave < raiz->n.chave){
        raiz->left = insere(raiz->left, x);
    }

    return raiz;//caso ja exista
}