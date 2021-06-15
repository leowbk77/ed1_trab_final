#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "imm.h"
#include "../Timg/Timg.h"


int file_type(char *file){
    
    int file_ext = strlen(file) - 3;

    if(!(strcmp(&file[file_ext], "imm"))){
        return BINARY; // binario
    }else if(!(strcmp(&file[file_ext], "txt"))){
        return TEXT; // texto
    }else return INVALID_FORMAT; // erro de formato invalido
}

int start_proc(int argcn, char *argval[]){
    if(argcn < 3 || argcn > 5) return INVALID_ARGUMENT;

    return call_proc(read_arg(argval[1]), argcn, argval);
}

int read_arg(char *argval){
    char *valid[5] = {"-open\0","-convert\0","-segment\0","-cc\0","-lab\0"};

    int indc = 0;
    int flag = -1; // flag inicia em -1; evita tentar dar open sempre; retorna INVALID_ARGUMENT

    while((indc < 5) && (flag == -1)){
        if(!(strcmp(argval, valid[indc]))){
            flag = indc;
        }
        indc++;
    }
    return flag; // retornar a flag
}

int call_proc(int flag, int argcn, char *argval[]){

    switch(flag){
        case OPEN:
            //file_type(argval[2]);//verificacao se arquivo binario ou txt
            //funcao de abertura do arquivo
            printf("comando: open");
            break;
        case CONVERT:
            //verifica os tipos dos arquivos
            //funcao que faz a conversao
            printf("comando: convert");
            break;
        case SEGMENT:
            //funcao que faz o thresholding
            printf("comando: segment");
            break;
        case CC:
            //funcao que detecta os componentes conexos
            printf("comando: cc");
            break;
        case LAB:
            //funcao que mostra o caminho no labirinto
            printf("comando: lab");
            break;
        default:
            return INVALID_ARGUMENT;
    }
    return SUCCESS; // temporario para testes
}

int open_txt(char *filepath){
    FILE *image = NULL;

    image = fopen(filepath, "r"); // abre o arquivo de imagem
    if(image == NULL){
        return INVALID_ARGUMENT;
    }

    char c;

    while (!feof(image)) {
        c = fgetc(image);

        if (c != EOF) 
            printf("%c", c);
    }

    fclose(image); // fecha o arquivo

    return SUCCESS;
}

int open_bin(char *filepath){
    FILE *image = NULL;

    image = fopen(filepath, "rb"); // abre o arquivo de imagem
    if(image == NULL){
        return INVALID_ARGUMENT;
    }

    int inteiro, largura, altura;
    
    fread(&largura, sizeof(int), 1, image);
    fread(&altura, sizeof(int), 1, image);

    for(int i = 0; i < altura; i++) {
        for(int j = 0; j < largura; j++) {
            fread(&inteiro, sizeof(int), 1, image);
            printf("%d ", inteiro);
        }
        printf("\n");
    }

    fclose(image); // fecha o arquivo

    return SUCCESS;
}

int convert(char *filepath, char *resultfile){

    img *p_img = NULL;
    FILE *image = NULL;

    image = fopen(filepath, "r"); // abre a imagem em formato .txt
    if(image == NULL) return INVALID_ARGUMENT;

    int rows = 0;
    int columns = 0;

    nrow_ncol(image, &rows, &columns); // descobre quantas linhas/colunas
    p_img = create_img(columns, rows); // aloca o espaco da imagem na memoria
    read_txt(p_img, image); // coloca os pixels na memoria (já em bin)

    fclose(image); // fecha o arquivo .txt

    image = fopen(resultfile, "wb"); // escreve/sobrescreve o novo arquivo binario
    if(image == NULL) return INVALID_ARGUMENT;

    write_bin(p_img, image, columns, rows); // coloca os pixels na memoria para o arquivo

    fclose(image); // fecha o arquivo

    free_img(p_img); // libera a memoria ocupada

    return SUCCESS;
}

int segment(char *thr, char *filepath, char *resultfile){
    int thr_int = 0;
    int largura = 0;
    int altura = 0;

    sscanf(thr, "%d", &thr_int); // pega o valor do thr passado por argumento

    FILE *fp = NULL;
    img *p_img = NULL;

    fp = fopen(filepath, "rb");
    if(fp == NULL) return INVALID_NULL_POINTER;

    p_img = read_bin(fp);
    if(p_img == NULL) return INVALID_NULL_POINTER;

    fread(&largura, sizeof(int), 1, fp);
    fread(&altura, sizeof(int), 1, fp);
    rewind(fp);

    fclose(fp);

    fp = fopen(resultfile, "wb");

    img_thr(p_img, thr_int);
    write_bin(p_img, fp, largura, altura);
    
    fclose(fp);

    return SUCCESS;
}