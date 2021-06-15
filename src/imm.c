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

            if(argcn != 3) return INVALID_ARG_COUNT;
            return open(file_type(argval[2]), argval[2]);
            break;

        case CONVERT:

            if(argcn != 4) return INVALID_ARG_COUNT;
            // primeiro argumento [2] : txt && segundo argumento [3] : bin 
            if((file_type(argval[2]) == 1) && (file_type(argval[3]) == 0)){
                return convert(argval[2], argval[3]); // converte .txt para bin
            }else{
                return INVALID_ARGUMENT;
            }
            break;

        case SEGMENT:

            if(argcn != 5) return INVALID_ARG_COUNT;
            // arquivo de entrada [3] : bin
            if(file_type(argval[3]) == 0){
                return segment(argval[2], argval[3], argval[4]); // limiariza a imagem
            }else{
                return INVALID_ARGUMENT;
            }
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
        
            printf("-6: Comando não encontrado.\n");
            return INVALID_ARGUMENT;
    }
    return SUCCESS; // temporario para testes
}

int open(int filetype, char *filepath){
    // verifica o tipo de entrada : txt ou bin
    if(filetype){
        return open_txt(filepath); // abre arquivo texto e exibe
    }else if(filetype == 0){
        return open_bin(filepath); // abre arquivo bin e exibe
    }else{
        return INVALID_ARGUMENT;
    }
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

        if (c != EOF) printf("%c", c);
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
    
    fread(&largura, sizeof(int), 1, image); // pega os valores de largura e altura
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

    fp = fopen(filepath, "rb"); // abre o arquivo binario
    if(fp == NULL) return INVALID_NULL_POINTER;

    p_img = read_bin(fp); // armazena na memoria as informacoes da imagem
    if(p_img == NULL) return INVALID_NULL_POINTER;

    resolution(p_img, &largura, &altura); // pega a resolucao da imagem
    fclose(fp);

    fp = fopen(resultfile, "wb"); // cria o novo arquivo
    if(fp == NULL) return INVALID_NULL_POINTER;

    img_thr(p_img, thr_int); // faz o thresholding da imagem
    write_bin(p_img, fp, largura, altura); // escreve o novo arquivo
    fclose(fp);

    return SUCCESS;
}