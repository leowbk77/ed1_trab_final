#include <stdlib.h>
#include <stdio.h>
#include "Timg.h"

struct matrice_data
{
    int width; // largura
    int height; // altura
    int *data; // dados
};

img *create_img(int w, int h){
    img *p_img = NULL;

    p_img = malloc(sizof(img));

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