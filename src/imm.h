#ifndef _immh_
#define _immh_

/*
*    DEFINICOES
*/

#define SUCCESS 0
#define INVALID_NULL_POINTER -1
#define OUT_OF_MEMORY -2
#define INVALID_ARGUMENT -3
#define ELEM_NOT_FOUND -4

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
int open_txt();

int open_bin();

#endif