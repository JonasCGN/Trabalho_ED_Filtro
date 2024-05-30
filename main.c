#include <stdio.h>
#include <stdlib.h>

#include "./functions/image.c"


int main(){
    FILE *arq; 
    arq = fopen("../utils/input_image_example_Gray.txt","r");
    if(!arq){
        printf("erro abrir arquivo\n");
    }

    ImageGray *image_Gray = create_image_gray(arq);
    if(image_Gray == NULL){
        printf("erro ao criar imagem cinza\n");
    }
    mostrar_imagem_Gray(image_Gray);

    return 0;


}