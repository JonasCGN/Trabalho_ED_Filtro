#include <stdio.h>
#include <stdlib.h>
#include "./functions/image.c"

int main(){
    FILE *arq;
    
    arq = fopen("../utils/input_image_example_Gray.txt","r");
    
    ImageGray *imageG = create_image_gray(arq);

    fclose(arq);

    mostrar_imagem_Gray(imageG);


    return 0;
}