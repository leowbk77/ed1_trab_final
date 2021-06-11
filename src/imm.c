#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "imm.h"
#include "../Timg/Timg.h"


int main(int argc, char *argv[]){
     // teste
    if(is_arg(1,argv)){
        printf("eh argumento no primeiro\n");
    }else{
        printf("nao eh argumento no primeiro\n");
    }
    
    return 0;
}

int is_arg(int argn, char *argval[]){

    if(argval[argn][0] != '-'){
        return 0;
    }else{
        return 1;  
    }

}

int read_arg(char *argval[]){
    char *valid[5] = {"-open\0","-convert\0","-segment\0","-cc\0","-lab\0"};

    int indc = 0;
    int flag = -1; // flag inicia em -1; evita tentar dar open sempre; retorna NOT_FOUND

    while((indc < 5) && (flag == -1)){
        if(!(strcmp(argval[1], valid[indc]))){
            flag = indc;
        }
        indc++;
    }


    /*
        SWITCH CASE QUE CHAMA AS FUNCOES DE PROCESSAMENTO 
        (provavelmente é melhor colocar em uma funcao separada repassando o valor da flag | ex: call_proc(int flag, char *argval[]))
    */

    switch(flag){
        case OPEN:
            file_type(argval[2]);//verificacao se arquivo binario ou txt
            //funcao de abertura do arquivo
            break;
        case CONVERT:
            //verifica os tipos dos arquivos
            //funcao que faz a conversao
            break;
        case SEGMENT:
            //funcao que faz o thresholding
            break;
        case CC:
            //funcao que detecta os componentes conexos
            break;
        case LAB:
            //funcao que mostra o caminho no labirinto
            break;

        default:
            printf("Erro! comando não encontrado.");
            return ELEM_NOT_FOUND;
    }

    return SUCCESS;
}

int file_type(char *file){
    
    int file_ext = strlen(file) - 3;

    if(!(strcmp(&file[file_ext], "imm"))){
        return 0; // binario
    }else if(!(strcmp(&file[file_ext], "txt"))){
        return 1; // texto
    }else return -1; // erro de formato invalido
}