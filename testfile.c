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

// funcoes de abertura do arquivo
int open_txt(char *filepath);

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

int set_pxl(img *p_img, int line, int column, int pixel){
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
            if(file_type(argval[2])){ // se for arquivo .txt abre a imagem
                return open_txt(argval[2]);
            }
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
    *ncolumn = ++col; // em alguns casos necessario ser ++col (corrigir)

    rewind(fp); // retorna o ponteiro do arquivo para o inicio
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

    rewind(fp); // retorna o ponteiro do arquivo para o inicio
    return SUCCESS;
}

int write_bin(img *img, FILE *fp, int width, int height){
    fwrite(&width, sizeof(int), 1, fp); // escreve a largura
    fwrite(&height, sizeof(int), 1, fp); // escreve a altura

    fwrite(img->data, sizeof(int), width * height, fp); // escreve o vetor de dados

    rewind(fp);
    return SUCCESS;
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

    // debug
    printf("\nIMAGEM DE ENTRADA:\n");
    printf("W: %d\tH: %d\n", columns, rows);
    img_print(p_img);
    printf("\n");
    // debug

    fclose(image); // fecha o arquivo .txt

    image = fopen(resultfile, "wb"); // escreve/sobrescreve o novo arquivo binario
    if(image == NULL) return INVALID_ARGUMENT;

    write_bin(p_img, image, columns, rows); // coloca os pixels na memoria para o arquivo

    fclose(image); // fecha o arquivo

    free_img(p_img); // libera a memoria ocupada

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

int main(int argc, char *argv[]){
    int teste = 0;
    // TESTE DO TAD IMAGEM
    /*
    img *p = NULL;
    
    p = create_img(2,2);
    if(p == NULL){
        printf("ERRO!\n");
        return 1;
    }
    
    set_pxl(p, 0, 0, 1);
    set_pxl(p, 0, 1, 0);
    set_pxl(p, 1, 0, 0);
    set_pxl(p, 1, 1, 1);
    
    img_print(p);
    
    int isfree = free_img(p);
    if(isfree == 0){
        printf("tudo certo com a imagem, fim de exec\n");
    }
    */
    // FIM TAD IMAGEM
    
    // TESTE DE ARGUMENTOS
    printf("%d é o numero de argumentos\n", argc);
    
    teste = start_proc(argc, argv);
    
    printf("\nteste : %d\n", teste);
    // FIM TESTE DE ARGUMENTOS
    
    // TESTE DAS FUNCOES DE LEITURA DO ARQUIVO 
    /*
    FILE *fp;
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("[ERRO]\n");
    } else if (fp != NULL) {
        printf("Arquivo aberto com sucesso.\n");
    }
	
    int col = 0, row = 0;
    nrow_ncol(fp, &row, &col);
	rewind(fp);
	
    printf("\nNúmero de linhas:%d\nNúmero de colunas:%d\n", row, col);

    p = create_img(col, row);
	
    write_txt(p, fp);
    img_print(p);
   
    fclose(fp);
    free_img(p);
    */
    // FIM TESTE DE LEITURA

    // TESTE DA FUNCAO DE CONVERSAO TXT BIN
    // teste = convert(argv[1], argv[2]);
    // printf("\nteste de conversao: %d\n", teste);
    // FIM TESTE CONVERSAO

    // TESTE DO OPEN_BIN
    teste = open_bin(argv[1]);
    printf("\nteste de open_bin: %d\n", teste);
	// FIM TESTE DO OPEN_BIN

    // TESTE DO SEGMENT
    // teste = segment(argv[2], argv[3], argv[4]);
    // printf("\nteste de segment: %d\n", teste);
	// FIM TESTE DO OPEN_BIN

    return 0;
}

/* IMAGENS PARA TESTE

5 x 3
76 101 111 255 255
255 87 98 75 255
255 55 55 255 255

3 x 3
0 0 1
1 0 0
0 1 0

3 x 2
1 0 1
0 1 0

5 x 3
1 0 1 0 1
0 1 0 1 0
1 0 1 0 1

*/