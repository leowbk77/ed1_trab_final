#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SUCCESS 0
#define INVALID_NULL_POINTER -1
#define OUT_OF_MEMORY -2
#define INVALID_ARGUMENT -3
#define ELEM_NOT_FOUND -4

typedef struct matrice_data img;

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

// inicio das chamadas de funcao
int start_proc(int argcn, char *argval[]);

// verifica qual argumento foi passado e chama funcao correspondente. (futuramente retornar so a flag)
int read_arg(char *argval);

// funcao que chama as funcoes de arcordo com a flag
int call_proc(int flag, int argcn, char *argval[]);

/*
**********************************************************************************
*/

struct matrice_data
{
    int width; // largura | linha
    int height; // altura | coluna
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

int pxl_set(img *p_img, int line, int column, int pixel){
    if(p_img == NULL) return INVALID_NULL_POINTER;

    int pos = line * p_img->width + column;
    p_img->data[pos] = pixel;
    
    return SUCCESS;
}

void img_print(img *p_img){
    
    int pos = 0;
    
    for(int i = 0; i < p_img->height; i++){
        for(int j = 0; j < p_img->width; j++){
            pos = i * p_img->width + j;
            printf("%d ", p_img->data[pos]);
        }
        printf("\n");
    }
}

/*
**********************************************************************************
*/

int is_arg(int argn, char *argval[]){

    if(argval[argn][0] != '-'){
        return 0;
    }else{
        return 1;  
    }
}

int file_type(char *file){
    
    int file_ext = strlen(file) - 3;

    if(!(strcmp(&file[file_ext], "imm"))){
        return 0; // binario
    }else if(!(strcmp(&file[file_ext], "txt"))){
        return 1; // texto
    }else return -1; // erro de formato invalido
}

int start_proc(int argcn, char *argval[]){
    if(argcn < 3 || argcn > 5) return INVALID_ARGUMENT;
    
    return call_proc(read_arg(argval[1]), argcn, argval);
}

int read_arg(char *argval){
    char *valid[5] = {"-open\0","-convert\0","-segment\0","-cc\0","-lab\0"};

    int indc = 0;
    int flag = -1; // flag inicia em -1; evita tentar dar open sempre; retorna NOT_FOUND

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
            ////funcao de abertura do arquivo
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
                pxl_set(img, row, col, pixel);
            }
            col++;
        }
        row++;
    }

    return SUCCESS;
}

int main(int argc, char *argv[]){
    
    // TESTE DO TAD IMAGEM
    img *p = NULL;
    
    p = create_img(2,2);
    if(p == NULL){
        printf("ERRO!\n");
        return 1;
    }
    
    pxl_set(p, 0, 0, 1);
    pxl_set(p, 0, 1, 0);
    pxl_set(p, 1, 0, 0);
    pxl_set(p, 1, 1, 1);
    
    img_print(p);
    
    int isfree = free_img(p);
    if(isfree == 0){
        printf("tudo certo com a imagem, fim de exec\n");
    }
    // FIM TAD IMAGEM
    
    // TESTE DE ARGUMENTOS
    printf("%d é o numero de argumentos\n", argc);
    
    int teste = start_proc(argc, argv);
    
    printf("\nteste : %d\n", teste);
    // FIM TESTE DE ARGUMENTOS
    
    FILE *fp;

    fp = fopen("./imagens/teste.txt", "r");
    if (fp == NULL) {
        printf("[ERRO]\n");
    } else if (fp != NULL) {
        printf("Arquivo aberto com sucesso.\n");
    }

    int col = 0, row = 0;
    nrow_ncol(fp, &row, &col);

    printf("\nNúmero de linhas:%d\nNúmero de colunas:%d\n", row, col);

    p = create_img(col, row);

    fclose(fp);
    fp = fopen("./imagens/teste.txt", "r");
    if (fp == NULL) {
        printf("[ERRO]\n");
    } else if (fp != NULL) {
        printf("Arquivo aberto com sucesso.\n");
    }

    write_txt(p, fp);

    img_print(p);
    
    fclose(fp);
    free_img(p);
    return 0;
}