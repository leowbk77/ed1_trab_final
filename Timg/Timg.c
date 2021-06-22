#include <stdlib.h>
#include <stdio.h>
#include "Timg.h"
#include "../Tlist/Tlist.h"

struct matrice_data
{
    int width; // largura | colunas
    int height; // altura | linhas
    int *data; // dados
};

img *create_img(int w, int h){
    img *p_img = NULL;

    p_img = malloc(sizeof(img));

    if(p_img != NULL){
        p_img->width = w;
        p_img->height = h;

        p_img->data = calloc(w*h, sizeof(int)); 
        if(p_img->data == NULL){
            free(p_img);
            p_img = NULL; 
        }
    }
    return p_img;
}

int free_img(img *p_img){
    if(p_img == NULL) return INVALID_NULL_POINTER;

    free(p_img->data);
    free(p_img);

    return SUCCESS;
}

int set_pxl(img *p_img, int line, int column, int pixel){
    if(p_img == NULL) return INVALID_NULL_POINTER;

    int pos = line * p_img->width + column;
    p_img->data[pos] = pixel;

    return SUCCESS;
}

int get_pxl(img *p_img, int line, int column, int *pixel){
    if(p_img == NULL) return INVALID_NULL_POINTER;

    int pos = line * p_img->width + column;
    *pixel = p_img->data[pos];

    return SUCCESS;
}

void img_print(img *p_img){
    int pos = 0;
    
    for(int i = 0; i < p_img->height; i++){
        for(int j = 0; j < p_img->width; j++){
            pos = i * p_img -> width + j;
            printf("%d ", p_img->data[pos]);
        }
        printf("\n");
    }
}

int nrow_ncol(FILE *fp, int *nrow, int *ncolumn) {
    int col = 0;
    int row = 0;
    char c;

    while (!feof(fp)) {
        c = fgetc(fp);
        if (c == '\n') {
            row++;
        }

        if (row == 0 && (c == ' ' || c == '\t')) {
            col++;
        } 
    }
    
    *nrow = ++row;
    *ncolumn = ++col;

    rewind(fp);
    return SUCCESS;
}

int read_txt(img *img, FILE *fp) {
    int pixel;

	for(int i = 0; i < img->height; i++){
		for(int j = 0; j < img->width; j++){
			fscanf(fp, "%d", &pixel);
			set_pxl(img, i, j, pixel);	
		}
	}

    rewind(fp);
    return SUCCESS;
}

int write_bin(img *img, FILE *fp, int width, int height){
    fwrite(&width, sizeof(int), 1, fp); // escreve a largura
    fwrite(&height, sizeof(int), 1, fp); // escreve a altura

    fwrite(img->data, sizeof(int), width * height, fp); // escreve o vetor de dados

    rewind(fp);
    return SUCCESS;
}

/* ARQUIVO BIN (.IMM)
                                |========|
    4 BYTES (INT)               |largura |
    4 BYTES (INT)               |altura  |
                                |--------|
    IMG->DATA (W*H * INT)       |pixels  |
                                |========|
*/

img *read_bin(FILE *fp){

    int largura = 0;
    int altura= 0;
    int pixel = 0;
    img *img = NULL;

    fread(&largura, sizeof(int), 1, fp);
    fread(&altura, sizeof(int), 1, fp);

    img = create_img(largura, altura);
    if(img == NULL) return NULL;

	for(int i = 0; i < img->height; i++){
		for(int j = 0; j < img->width; j++){
            fread(&pixel, sizeof(int), 1, fp);
			set_pxl(img, i, j, pixel);	
		}
	}

    rewind(fp);
    return img;
}

int img_thr(img *p_img, int thr){

    int pos = 0;

    for(int i = 0; i < p_img->height; i++){
        for(int j = 0; j < p_img->width; j++){
            pos = i * p_img -> width + j;

            if(p_img->data[pos] > thr){
                p_img->data[pos] = 1;
            }else{
                p_img->data[pos] = 0;
            }
        }
    }

    return SUCCESS;
}

int resolution(img *p_img, int *width, int *height){
    if(p_img == NULL) return INVALID_NULL_POINTER;

    *width = p_img->width;
    *height = p_img->height;
    
    return SUCCESS;
}

int img_rotule(img *p_img, char *filepath) {
    if (p_img == NULL) return INVALID_NULL_POINTER;

    img *im_rot;
    im_rot = create_img(p_img -> width, p_img -> height);
    if (im_rot == NULL) return INVALID_NULL_POINTER;

    int label = 1;
    list *li;
    li = list_create();
    ponto p, p_atual;
    ponto lista_de_pontos[] = {{0,-1},{0,1},{-1,0},{1,0}};

    int pixel_im;
    int pixel_im_rot;

    for(int i = 1; i < p_img -> height - 1; i++) {
        for(int j = 1; j < p_img -> width - 1; j++) {
            p.x = i;
            p.y = j;

            get_pxl(p_img, p.x, p.y, &pixel_im);
            get_pxl(im_rot, p.x, p.y, &pixel_im_rot);

            if (pixel_im == 1 && pixel_im_rot == 0) {
                set_pxl(im_rot, p.x, p.y, label);

                push(li, p);

                while(!is_empty(li)) {
                    pop(li, &p_atual);

                    for(int i = 0; i < 4; i++) {
                        p.x = p_atual.x + lista_de_pontos[i].x;
                        p.y = p_atual.y + lista_de_pontos[i].y;

                        get_pxl(p_img, p.x, p.y, &pixel_im);
                        get_pxl(im_rot, p.x, p.y, &pixel_im_rot);

                        if (pixel_im == 1 && pixel_im_rot == 0) {
                            set_pxl(im_rot, p.x, p.y, label);

                            push(li, p);
                        }
                    }   
                }
                label++;
            }
        }
    }
    FILE *fp;
    fp = fopen(filepath, "wb");
    write_bin(im_rot, fp, p_img -> width, p_img -> height); // escreve no arquivo a path rotulada
    fclose(fp);

    list_free(li);
    free_img(im_rot);
    return SUCCESS;
}

int find_route(img *p_img, char *filepath) {
    if (p_img == NULL) return INVALID_NULL_POINTER;

    int largura = p_img -> width;
    int altura = p_img -> height;

    list *li;
    li = list_create();
    ponto p, p_atual; // para percorrer os vizinhos
    ponto lista_de_pontos[] = {{0,-1},{0,1},{-1,0},{1,0}}; // cordenadas dos vizinhos
    ponto p_inicial, p_final; // ponto inicial do labirinto e ponto final do labirinto

    int pixel;

    for(int i = 0; i < altura; i++) {   // pegando o ponto inicial do labirinto
        get_pxl(p_img, i, 0, &pixel);

        if (pixel != 0) {
            p_inicial.x = 0;
            p_inicial.y = i;
        }
    }

    pixel = 0;

    for(int i = 0; i < altura; i++) {   // pegando o ponto final do labirinto
        get_pxl(p_img, i, largura - 1, &pixel);

        if (pixel != 0) {
            p_final.x = largura - 1;
            p_final.y = i;
        }
    }

    p.x = 1;    // setando o p para o próximo elemento após p ponto inicial
    p.y = p_inicial.y;

    set_pxl(p_img, p_inicial.y, p_inicial.x, 2); // setando ponto inicial para 2, já que faz parte do caminho

    int unv_side = 0; // contador de lados validos observados

    push(li, p);

    while (1) {
        p_atual = p;

        for(int i = 0; i < 4; i++) {

            p.x = p_atual.x + lista_de_pontos[i].x;
            p.y = p_atual.y + lista_de_pontos[i].y;

            get_pxl(p_img, p.y, p.x, &pixel);

            if (pixel == 1) {
                push(li, p);
                set_pxl(p_img, p.y, p.x, 3);
                i = -1; // reseta o contador (p se moveu para o prox pixel)
                p_atual = p; // evita voltar pro comeco
                unv_side = 0; // lados validos 
            } else {
                p = p_atual;
                unv_side++;
            }
        }   

        if (p.x == p_final.x && p.y == p_final.y) {
            break;
        }

        if(unv_side == 4){ // pop na lista até o ultimo "cruzamento"
            pop(li, &p);
            top(li, &p);
            unv_side = 0;
        }
    }

    // linha comentada para teste
    list_insert_front(li, p_inicial); // inserindo ponto inicial no começo da lista

    list_print(li); // printando lista com o caminho

    while (!is_empty(li)) { 
       pop(li, &p_atual);
       set_pxl(p_img, p_atual.y, p_atual.x, 2);
    }

    limpar_imagem_lab(p_img); // limpando "rabiscos" usados para encontrar o caminho correto

    FILE *fp;
    fp = fopen(filepath, "wb");
    write_bin(p_img, fp, largura, altura); // escreve no arquivo a path rotulada
    fclose(fp);

    list_free(li);
    return SUCCESS;
}

int limpar_imagem_lab(img *p_img) {
    if (p_img == NULL) return INVALID_NULL_POINTER;

    int pxl;

    for(int i = 0; i < p_img -> height; i++) {
        for(int j = 0; j < p_img -> width; j++) {
            get_pxl(p_img, i, j, &pxl);

            if (pxl == 3) {
                set_pxl(p_img, i, j, 1);
            }
        }
    }

    return SUCCESS;
}