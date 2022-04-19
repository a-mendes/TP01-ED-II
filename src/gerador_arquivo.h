#ifndef gerador_arquivo_h
#define gerador_arquivo_h

typedef struct {
	int chave;
	long long dado1;
	char dado2[5000]; 
} TRegistro;

// 1
char* gerarArquivoAscendente(int numeroRegistros);

// 2
char* gerarArquivoDescendente(int numeroRegistros);

// 3
char* gerarArquivoAleatorio(int numeroRegistros);

#endif