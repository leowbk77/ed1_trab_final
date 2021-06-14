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
