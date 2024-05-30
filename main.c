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
    
    printf("------Histograma------\n");
    for(int i=0;i<COR;i++){
        printf("%d - %d\n", i,histograma[i]);
    }
    printf("\n");

    int *hv = calculaHv(histograma,64);

    

    free(hv);

    return 0;
}