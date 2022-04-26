#include "binaria.h"

//inicia a pesquisa
void binaria(int quantidade, int situacao, int chave, int opcional){
    Noh *t; //Arvore Binaria
    TRegistro *aux;  // item para leitura de registros
    double tamanho;  // tamanho da tabela de índices
    char *registros;  // arquivo com a quantidade de registros informada

    // variáveis para medir o tempo de execução
    clock_t startIndice, endIndice;
    double time;
    
    // Gera o arquivo com a quantidade de registros informada
    switch(situacao){
        case 1: 
            registros = gerarArquivoAscendente(quantidade);
            break;
        case 2: 
            registros = gerarArquivoDescendente(quantidade);
            break;
        case 3: 
            registros = gerarArquivoAleatorio(quantidade);
            break;
        default:
            registros = gerarArquivoAleatorio(quantidade);
            break;
    }

    // Abre o arquivo de registros
    FILE *arquivo = fopen(registros, "rb");

    //impressao de itens
    if (opcional) 
        printaRegistros(quantidade, arquivo);

    //alocando memoria pra variavel de registros
    aux = (TRegistro *)malloc(quantidade * sizeof(TRegistro));
    
    // inicia a contagem do tempo
    startIndice = clock();  

    //lendo os registros do livro
    fread(aux, sizeof(TRegistro), quantidade, arquivo);

    //Iniciando a arvore
    t = criarNoh(aux[0]); 

    for(int i = 0; i < quantidade; i++)
        insere(t, aux[i+1]);  // monta a arvore

    // finaliza o cronômetro
    endIndice = clock();                          

    time = ((double)(endIndice - startIndice)) / CLOCKS_PER_SEC;    // calcula o tempo de execução
    printf("Tempo de execucao (criacao de indices): %lf\n", time);  // imprime o tempo de execução

    printarResultado(pesquisar(t, chave), chave); // impressao do resultado da busca
    
    free(t);      // libera a arvore
    free(aux);    // libera o auxiliar

    fclose(arquivo);  // fecha o arquivo de registros
}

//rodar 20x com numeros aleatorios
void binariaRandomica(int quantidade, int situacao, int chave, int opcional){
    int n = 20;

    while(n > 0){
        chave = getRandomNumber();
        binaria(quantidade, situacao, chave, opcional);
        n--;
    }
}

//pesquisar percorrendo a árvore
Noh* pesquisar(Noh *raiz, int x)
{
    if(raiz == NULL) //caso seja nulo
        return NULL;

    if(raiz->n.chave == x) //caso encontre
        return raiz;
    else if(x < raiz->n.chave) // caso seja menor que a pesquisa, percorre recursivamente pra esquerda 
        return pesquisar(raiz->left, x);
    else // caso seja maior que a pesquisa, percorre recursivamente pra direita 
        return pesquisar(raiz->right, x);
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
    if(raiz == NULL)
        raiz = criarNoh(x);
    else if(x.chave > raiz->n.chave)
        raiz->right = insere(raiz->right, x); //Caso a chave seja maior, insere no filho direito
    else if(x.chave < raiz->n.chave)
        raiz->left = insere(raiz->left, x); //Caso a chave seja menor, insere no filho esquerdo

    return raiz;//caso ja exista
}

void printarResultado(Noh* resultado, int chave){
    if (resultado != NULL) {
        printf("Registro encontrado!\n");
        printf("Chave: %d\n", resultado->n.chave);
        printf("Valor: %ld\n", resultado->n.dado1);
        printf("Nome: %s\n\n", resultado->n.dado2);
    } else {
        printf("Registro nao encontrado!\n");
        printf("Chave: %d\n\n", chave);
    }
}