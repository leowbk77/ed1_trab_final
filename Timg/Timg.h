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

// funcao que printa a imagem (para testes)
void img_print(img *p_img);

// funcoes de escrever do arquivo para a memoria
int write_txt(img *img, FILE *fp);

int write_bin(img *img, FILE *fp);

// funcao que retorno o numero de linhas e colunas de uma imagem txt
int nrow_ncol(FILE *fp, int *nrow, int *ncolumn);

#endif