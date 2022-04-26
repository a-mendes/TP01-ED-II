#include "binaria.h"

//inicia a pesquisa
void binaria(int quantidade, int situacao, int chave, int opcional){
    Noh *t; //Arvore Binaria
    TRegistro *aux;  // item para leitura de registros
    double tamanho;  // tamanho da tabela de índices
    char *registros;  // arquivo com a quantidade de registros informada
    OpCounter operacoes = (OpCounter){0}; //Struct para contar as operações
    
    // variáveis para medir o tempo de execução
    clock_t startIndice, endIndice, start2Indice, end2Indice;
    double time, time2;

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
    
    //caso seja o caso randomico
    if(chave == -1){
        binariaRandomica(quantidade, situacao, arquivo);
        return;
    }

    //lendo os registros do livro
    fread(aux, sizeof(TRegistro), quantidade, arquivo);

    //Iniciando a arvore
    t = criarNoh(aux[0], &operacoes); 
    
    startIndice = clock();
    for(int i = 0; i < quantidade; i++)
        insere(t, aux[i+1], &operacoes);  // monta a arvore
    
    endIndice = clock();  

    time = ((double)(endIndice - startIndice)) / CLOCKS_PER_SEC;    // calcula o tempo de execução

    //pesquisando e pegando o tempo de pesquisa
    start2Indice = clock();

    Noh* pesquisa = pesquisar(t, chave, &operacoes);

    end2Indice = clock();  

    time2 = ((double)(end2Indice - start2Indice)) / CLOCKS_PER_SEC;    // calcula o tempo de pesquisa
    

    printarResultado(pesquisa, chave); // impressao do resultado da busca
    // imprime o tempo de execução
    printf("Tempo de execucao (criacao de indices): %lfs\n", time);  
    // imprime o tempo de pesquisa
    printf("\nTempo de execucao (pesquisa no arquivo): %lfs\nNumero de transferencias: %ld\nNumero de comparacoes: %ld\n\n", time2, operacoes.transfers, operacoes.comparisons);
    finaliza(t, aux, arquivo); //encerra as variaveis
}

//rodar 20x com numeros aleatorios
void binariaRandomica(int quantidade, int situacao, FILE* arquivo){
    int chaves[20];
    obter20RegistrosAleatorios(arquivo, quantidade, chaves);
    
    for (int i = 0; i < 20; i++) 
        binaria(quantidade, situacao, chaves[i], 0);
                     
}

//pesquisar percorrendo a árvore
Noh* pesquisar(Noh *raiz, int x, OpCounter *operacoes)
{
    operacoes->comparisons++;
    if(raiz == NULL) //caso seja nulo
        return NULL;

    operacoes->comparisons++;
    if(raiz->n.chave == x) //caso encontre
        return raiz;

    operacoes->comparisons++;
    if(x < raiz->n.chave) // caso seja menor que a pesquisa, percorre recursivamente pra esquerda 
        return pesquisar(raiz->left, x, operacoes);

    operacoes->comparisons++;
    if(x > raiz->n.chave) // caso seja maior que a pesquisa, percorre recursivamente pra direita 
        return pesquisar(raiz->right, x, operacoes);
}

//inicializa um nó
Noh* criarNoh(TRegistro x, OpCounter *operacoes)
{
    Noh* no = (Noh*) malloc(sizeof(Noh));
    
    no->n = x; //inserindo o valor x
    no->left = NULL; //inicializando o filho esquerdo como nulo
    no->right = NULL; //inicializando o filho direito como nulo

    operacoes->transfers++; //incrementa uma transferencia
    return no;
}

Noh* insere(Noh *raiz, TRegistro x, OpCounter *operacoes)
{
    if(raiz == NULL)
        raiz = criarNoh(x, operacoes);
    else if(x.chave > raiz->n.chave)
        raiz->right = insere(raiz->right, x, operacoes); //Caso a chave seja maior, insere no filho direito
    else if(x.chave < raiz->n.chave)
        raiz->left = insere(raiz->left, x, operacoes); //Caso a chave seja menor, insere no filho esquerdo

    return raiz;//caso ja exista
}

void printarResultado(Noh* resultado, int chave){
    printf("\n---------------------------------------------\n");
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

void finaliza(Noh* t, TRegistro* aux, FILE* arquivo){
    free(t);      // libera a arvore
    free(aux);    // libera o auxiliar
    fclose(arquivo);  // fecha o arquivo de registros
}