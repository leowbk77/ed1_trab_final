#ifndef _immh_
#define _immh_

/*
*    DEFINICOES
*/

#define INVALID_ARGUMENT -3

// indices
#define OPEN 0
#define CONVERT 1
#define SEGMENT 2
#define CC 3
#define LAB 4

// tipos de arquivo
#define BINARY 0
#define TEXT 1
#define INVALID_FORMAT -1

/*
*   FUNCOES 
*/

/*
*    Testes
*/

// verifica o tipo do arquivo | retorna 0 se binario; 1 se txt; -1 se invalido.
int file_type(char *file);

/*
*    Processamento
*/

// inicio das chamadas de funcao
int start_proc(int argcn, char *argval[]);

// verifica qual argumento foi passado e chama funcao correspondente. (futuramente retornar so a flag)
int read_arg(char *argval);

// funcao que chama as funcoes de arcordo com a flag
int call_proc(int flag, int argcn, char *argval[]);

// funcoes de abertura do arquivo
// abre a imagem em formato .txt
int open_txt(char *filepath);

// abre a imagem em formato binario
int open_bin(char *filepath);

// converte a imagem de .txt para .mmm | filepath : caminho do arquivo .txt  resultfile : o arquivo resultado a ser gerado
int convert(char *filepath, char *resultfile);

// faz a limiarização da imagem
int segment(char *thr, char *filepath, char *resultfile);

#endif