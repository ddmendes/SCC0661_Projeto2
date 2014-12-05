#include "diff.h"
#define BIT_LEN 9

void diff(int * amostras, int * result, int size){
    int diff=0;
    int i;
    
    result[0] = amostras[0];

    for (i=1; i < size; i++) {
        printf("%d\n", i);
        result[i] = amostras[i] - amostras[i-1];
    }
}

void int2bin (int a, char * buffer, int size) {
    buffer += (size - 1);
    
    char aux[BIT_LEN];
    
    int j, l, k;
    for (j = (size-1); j >= 0; j--) {
        *buffer-- = (a & 1) + '0';
        
        a >>= 1;
    }
}

int main(int argc, char * argv[]){
    int amostras[] = {15, 10, 12, 8, 9, 8, 8, 0, 0, 0, 0};
    int * result;
    
    //NOTA: sizeof(array)/sizeof(type) só funciona com o array original, se for parametro de função pode não funcionar, adicionar como parametro se transformar esta main em função
    result = (int *) malloc(sizeof(amostras)/sizeof(int));
    
    diff(amostras, result, sizeof(amostras)/sizeof(int));
    
    
    
    
    
//    //transforma um inteiro decimal em string de zeros e uns
//    //baseado em http://stackoverflow.com/questions/1024389/print-an-int-in-binary-representation-using-c
//    char buffer[BIT_LEN+1];
//    buffer[BIT_LEN] = '\0';
//    
//    int sortudo = 2, k;
//    int2bin(amostras[sortudo], buffer, BIT_LEN - 1);
//    printf("O valor de result[%d]=%d em binário é\n", sortudo, result[sortudo]);
//    /*teste*/
//    for (k=0; (k < sizeof(buffer)/sizeof(char) && buffer[k] != '\0'); k++) {
//        printf("%c", buffer[k]);
//    }
//    printf("\n\n");
//    /**/
    
    
    return 0;
}