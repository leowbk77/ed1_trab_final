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
    if(argcn < 3 || argcn > 5) { // verificacao inicial se o numero de argumentos é válido
        printf("-5: Número de argumentos incorreto\n");
        return INVALID_ARG_COUNT;
    }
    
    // chama a funcao a ser executada de acordo com o comando passado
    return call_proc(read_arg(argval[1]), argcn, argval);
}

int read_arg(char *argval){
    char *valid[5] = {"-open\0","-convert\0","-segment\0","-cc\0","-lab\0"}; // lista de comandos válidos

    int indc = 0;
    int flag = -1; // flag inicia em -1; evita tentar dar open sempre; retorna INVALID_ARGUMENT na call_proc()

    // laço de comparacao da string (argv[1]) com os comandos existentes; seta a flag ao encontrar um comando válido
    while((indc < 5) && (flag == -1)){
        if(!(strcmp(argval, valid[indc]))){
            flag = indc;
        }
        indc++;
    }
    return flag; // retorna a flag
}

/*  Formato de entrada (*argv[])
    ./programa  arg1 arg2 arg3 arg4
        [0]     [1]  [2]   [3]  [4]
    [0] - caminho de excucao do executavel.
    [1] - arg1: comandos válidos do programa.
    [2] - arg2: caminho de arquivo de entrada (open, convert, cc, lab); argumento de thr (segment).
    [3] - arg3: caminho de arquivo de saída (convert, cc, lab); caminho de arquivo de entrada (segmnet).
    [4] - arg4: caminho de arquivo de saída (segment).
*/

int call_proc(int flag, int argcn, char *argval[]){

    switch(flag){
        case OPEN:

            if(argcn != 3) return INVALID_ARG_COUNT;
            return open(file_type(argval[2]), argval[2]); // exibe a imagem de acordo com o file_type
            break;

        case CONVERT:

            if(argcn != 4) return INVALID_ARG_COUNT;
            // primeiro argumento [2] : txt && segundo argumento [3] : bin 
            if((file_type(argval[2]) == TEXT) && (file_type(argval[3]) == BINARY)){
                return convert(argval[2], argval[3]); // converte .txt para bin
            }else{
                return INVALID_ARGUMENT;
            }
            break;

        case SEGMENT:

            if(argcn != 5) return INVALID_ARG_COUNT;
            // arquivo de entrada [3] : bin
            if(file_type(argval[3]) == BINARY){
                return segment(argval[2], argval[3], argval[4]); // limiariza a imagem
            }else{
                return INVALID_ARGUMENT;
            }
            break;

        case CC:

            if (argcn != 4) return INVALID_ARG_COUNT; 

            // ambos os arquivos devem ser .imm
            if (file_type(argval[2]) == BINARY && (file_type(argval[3]) == BINARY)) {
                return cc(argval[2], argval[3]); // rotula a imagem
            } else {
                return INVALID_ARGUMENT;
            }
            break;

        case LAB:

            if (argcn != 4) return INVALID_ARG_COUNT; 

            // verificando se arquivo de entrada é bin/txt e o de saída é bin
            if ((file_type(argval[2]) == BINARY) || (file_type(argval[2]) == TEXT) && file_type(argval[3]) == BINARY) {
                return lab(argval[2], argval[3]); // descobre o caminho do labirinto
            } else {
                return INVALID_ARGUMENT;
            }
            break;

        default:
            // flag retornada não é um comando válido
            printf("-6: Comando não encontrado.\n");
            return INVALID_ARGUMENT;
    }
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

    image = fopen(filepath, "r"); // abre o arquivo de imagem .txt
    if(image == NULL){
        return INVALID_ARGUMENT;
    }

    char c;
    // le os valores e printa em sequencia
    while (!feof(image)) {
        c = fgetc(image);

        if (c != EOF) printf("%c", c);
    }
    printf("\n");

    fclose(image); // fecha o arquivo

    return SUCCESS;
}

int open_bin(char *filepath){
    FILE *image = NULL;

    image = fopen(filepath, "rb"); // abre o arquivo de imagem binária .imm
    if(image == NULL){
        return INVALID_ARGUMENT;
    }

    int inteiro, largura, altura;
    
    fread(&largura, sizeof(int), 1, image); // pega os valores de largura e altura
    fread(&altura, sizeof(int), 1, image);
    // le os valores e printa em sequencia
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

    nrow_ncol(image, &rows, &columns); // descobre quantas linhas/colunas do arquivo .txt ('\n' e ' ')
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

    sscanf(thr, "%d", &thr_int); // pega o valor do thr passado por argumento (char -> int)

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

    fclose(fp); // fecha o arquivo
    free_img(p_img); // libera a imagem da memoria

    return SUCCESS;
}

int cc(char *filepath, char *resultfile) {
    FILE *fp = NULL;
    img *p_img = NULL;

    fp = fopen(filepath, "rb"); // abre o arquivo binário
    if (fp == NULL) return INVALID_ARGUMENT;

    p_img = read_bin(fp); // armazena do arquivo para a memória
    if(p_img == NULL) {
        return OUT_OF_MEMORY;
    }

    fclose(fp); // fecha o arquivo binario

    img_rotule(p_img, resultfile); // rotula a imagem do arq path e escreve no arq result

    free_img(p_img); // libera o espaço de memória da imagem

    return SUCCESS;
}

int lab(char *filepath, char *resultfile) {
    FILE *fp = NULL;
    img *p_img = NULL;

    int tipo_arquivo = file_type(filepath); // 1 = texto, 0 = binário, -1 = inválido 
    if (tipo_arquivo == INVALID_FORMAT) return INVALID_FORMAT;

    if (tipo_arquivo == BINARY) {
        fp = fopen(filepath, "rb"); // abre o arquivo binário
        if (fp == NULL) return INVALID_ARGUMENT;

        p_img = read_bin(fp); // armazena do arquivo para a memória
        if(p_img == NULL) {
            return OUT_OF_MEMORY;
        }
    } 

    if (tipo_arquivo == TEXT) {
        fp = fopen(filepath, "r"); // abre arquivo txt
        if (fp == NULL) return INVALID_ARGUMENT;

        int larg, alt;

        nrow_ncol(fp, &alt, &larg);

        p_img = create_img(larg, alt);

        read_txt(p_img, fp); 

        if(p_img == NULL) {
            return OUT_OF_MEMORY;
        }
    }

    fclose(fp); // fecha o arquivo lido para armazenar a imagem

    find_route(p_img, resultfile); // encontra o caminho correto do labirinto

    free_img(p_img); // libera o espaço de memória da imagem

    return SUCCESS;
}

int main(int argc, char *argv[]) {
    start_proc(argc, argv);
    return 0;
}