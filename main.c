#include <stdio.h>
#include <stdlib.h>
#include "./functions/image.c"

int main(){
    int dados[] = {
        52, 55, 61, 59, 79, 61, 76, 61,
        62, 59, 55, 104, 94, 85, 59, 71,
        63, 65, 66, 113, 144, 104, 63, 72,
        64, 70, 70, 126, 154, 109, 71, 69,
        67, 73, 68, 106, 122, 88, 68, 68,
        68, 79, 60, 70, 77, 66, 58, 75,
        69, 85, 64, 58, 55, 61, 65, 83,
        70, 87, 69, 68, 65, 73, 78, 90
    };

    int histograma[256] = {0};

    for(int i=0;i < 64;i++){
        int valor = dados[i];
        histograma[valor]++;
    }
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            printf("\033[38;2;%d;%d;%dm**\033[0m", dados[i * 8 + j], dados[i * 8 + j], dados[i * 8 + j]);
        }   
        printf("\n");

    }

    printf("\n");

    int *hv = calculaHv(histograma,64);

    int dadosNovo[64];
    for(int i=0;i<COR;i++){
        for(int j=0;j<64;j++){
            if(dados[j] == i){
                dadosNovo[j] = hv[i];
            }
        }
    }


    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            printf("\033[38;2;%d;%d;%dm**\033[0m", dadosNovo[i * 8 + j], dadosNovo[i * 8 + j], dadosNovo[i * 8 + j]);
        }   
        printf("\n");
    }
    

    free(hv);

    return 0;
}