#ifndef _immh_
#define _immh_

/*
*    DEFINICOES
*/

#define SUCCESS 0
#define INVALID_NULL_POINTER -1
#define OUT_OF_MEMORY -2
#define OUT_OF_RANGE -3
#define ELEM_NOT_FOUND -4

// indices
#define OPEN 0
#define CONVERT 1
#define SEGMENT 2
#define CC 3
#define LAB 4

/*
*   FUNCOES 
*/

/*
*    Testes
*/

// verifica se eh argumento valido na posicao argcn | devolve 1 se argumento. (funcao para teste do *argv[])
int is_arg(int argcn, char *argval[]); 

// verifica o tipo do arquivo | retorna 0 se binario; 1 se txt; -1 se invalido.
int file_type(char *file);

/*
*    Processamento
*/

// verifica qual argumento foi passado e chama funcao correspondente. (futuramente retornar so a flag)
int read_arg(char *argval[]);

#endif