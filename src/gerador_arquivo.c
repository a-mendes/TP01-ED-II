#include "gerador_arquivo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 *  arquivos contendo 100, 200, 2.000, 20.000, 200.000 e 2.000.000 registros
 * arquivos ordenados ascendentemente, descendentemente e aleatoriamente
*/

void montarNomeArquivo(char* nomeArquivo, int numeroRegistros, int situacao) {
	/** Definindo nome do arquivo
	 * 		"<pasta>/<tipo_de_ordenação>_<quantidade_de_registros>.dat"
	 */ 
	strcpy(nomeArquivo, "data/");

	static char numeroRegistrosString[10]; 
	itoa(numeroRegistros, numeroRegistrosString, 10); //Converte int para string

	static char situacaoOrdenacao[15];
	switch(situacao) {
		case 1:
			strcpy(situacaoOrdenacao, "ascendente_");
			break;
		case 2:
			strcpy(situacaoOrdenacao, "descendente_");
			break;

		case 3:
			strcpy(situacaoOrdenacao, "aleatorio_");
			break;
	}
	
	strcat(nomeArquivo, situacaoOrdenacao);
	strcat(nomeArquivo, numeroRegistrosString);
	strcat(nomeArquivo, ".dat");
}

const char* gerarArquivoAscendente(int numeroRegistros) {
	static char nomeArquivo[40];
	montarNomeArquivo(nomeArquivo, numeroRegistros, 1);

	/** Verificar se já existe um arquivo aleatório para a quanidade de registros solicitada 
	 * 		Se sim, ordenar o arquivo já existente
	 * 		Se não, criar um arquivo aleatório e ordená-lo
	 */ 

	return nomeArquivo;
}

const char* gerarArquivoDescendente(int numeroRegistros){
	static char nomeArquivo[40];
	montarNomeArquivo(nomeArquivo, numeroRegistros, 2);

	/** Verificar se já existe um arquivo aleatório para a quanidade de registros solicitada 
	 * 		Se sim, ordenar o arquivo já existente
	 * 		Se não, criar um arquivo aleatório e ordená-lo
	 */ 

	return nomeArquivo;
}

const char* gerarArquivoAleatorio(int numeroRegistros) {
	
	static char nomeArquivo[40];
	montarNomeArquivo(nomeArquivo, numeroRegistros, 3);

	/** Criando Arquivo 
	 * 		geração randomica de´números de 1 a 1000
	 */
	FILE *arquivo;
	arquivo = fopen(nomeArquivo, "wb"); // Cria um arquivo binário para gravação

    if (arquivo == NULL) {
        printf("Falha na geracao do arquivo externo\n");
        return "0";
    }

    //Randomizando chaves
    int chaves[numeroRegistros];
    srand(time(NULL));
    for (int i = 0; i < numeroRegistros; i++){
    	chaves[i] = rand() % 1000;
    }
    
    fwrite (chaves, sizeof(int), numeroRegistros, arquivo);

    fclose(arquivo);

    return nomeArquivo;
}