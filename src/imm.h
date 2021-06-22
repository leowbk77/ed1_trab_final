#ifndef _immh_
#define _immh_

/*
*    DEFINICOES
*/

#define INVALID_ARG_COUNT -5
#define INVALID_ARGUMENT -6

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

// inicio das chamadas de funcao | funcao "raiz" do programa
int start_proc(int argcn, char *argval[]);

// verifica qual argumento foi passado e retorna a flag correspondente de acordo com a tabela de indices
int read_arg(char *argval);

// funcao que chama as funcoes de acordo com a flag retornada pela read_arg
int call_proc(int flag, int argcn, char *argval[]);

// funcoes de visualizacao dos arquivos
// faz a chamada das funcoes de acordo com o tipo de arquivo
int open(int filetype, char *filepath);
// exibe a imagem em formato .txt
int open_txt(char *filepath);
// exibe a imagem em formato binario
int open_bin(char *filepath);

// converte a imagem de .txt para .mmm | filepath : caminho do arquivo .txt  resultfile : o arquivo resultado a ser gerado
int convert(char *filepath, char *resultfile);

// faz a limiarização da imagem
int segment(char *thr, char *filepath, char *resultfile);

// faz a rotulação de componentes conexos da imagem
int cc(char *filepath, char *resultfile);

// função que encontra o caminho do labirinto
int lab(char *filepath, char *resultfile);

#endif