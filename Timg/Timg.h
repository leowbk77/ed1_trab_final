#ifndef _timgh_
#define _timgh_

/*
*   DEFINES
*/

#define SUCCESS 0
#define INVALID_NULL_POINTER -1
#define OUT_OF_MEMORY -2
#define OUT_OF_RANGE -3
#define ELEM_NOT_FOUND -4

typedef struct matrice_data img;

/*
*   FUNCOES
*/

// funcao que aloca espaco da imagem | w : largura   h : altura (px)
img *create_img(int w, int h);

// funcao que libera o espaco alocado para a imagem
int free_img(img *p_img);

// funcao que seta o valor de determinado pixel
int set_pxl(img *p_img, int line, int column, int pixel);

// funcao que devolve o valor de determinado pixel
int get_pxl(img *p_img, int line, int column, int *pixel);

// funcao que printa a imagem
void img_print(img *p_img);

// funcoes que escrevem do arquivo para a memoria
// le o arquivo .txt e armazena na memoria
int read_txt(img *img, FILE *fp);

// le o arquivo .imm e armazena na memoria | devolve o ponteiro pro TAD imagem na memoria
img *read_bin(FILE *fp);

// funcao que altera a imagem de acordo com o valor do thr
int img_thr(img *p_img, int thr);

// escreve o arquivo binario | width : largura   height : altura 
int write_bin(img *img, FILE *fp, int width, int height);

// funcao que retorno o numero de linhas e colunas de uma imagem txt
int nrow_ncol(FILE *fp, int *nrow, int *ncolumn);

// funcao que retorna a largura e altura da imagem armazenada na memoria
int resolution(img *p_img, int *width, int *height);

// funcao que rotula a imagem passada no parametro
int img_rotule(img *p_img, char *filepath);

// funcao que encontra o caminho correto do labirinto e escreve no filepath 
int find_route(img *p_img, char *filepath);

// limpa a imagem apos encontrarmos o caminho
int limpar_imagem_lab(img *p_img);

#endif