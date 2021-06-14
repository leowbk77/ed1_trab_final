#include <stdlib.h>
#include <stdio.h>
#include "Timg.h"

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

        p_img->data = malloc((w*h)*(sizeof(int)));
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
    
    for(int i = 0; i < p_img->width; i++){
        for(int j = 0; j < p_img->height; j++){
            pos = i * p_img->width + j;
            printf("%d ", p_img->data[pos]);
        }
        printf("\n");
    }
}

int nrow_ncol(FILE *fp, int *nrow, int *ncolumn) {
    if (fp == NULL) {
        return INVALID_NULL_POINTER;
    }

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
    *ncolumn = col;

    return SUCCESS;
}

int write_txt(img *img, FILE *fp) {
    int col = 0, row = 0;
    int pixel;

    while(row < img -> height) {
        while (col < img -> width) {
            fscanf(fp, "%d", &pixel);

            if(pixel != ' ' && pixel != '\t') {
                set_pxl(img, row, col, pixel);
            }
            col++;
        }
        row++;
        col = 0;
    }

    return SUCCESS;
}