#include <stdio.h>
#include <stdlib.h>

#include "./functions/image.c"

int num = 1;


int main(){
    printf("oi");
    FILE *arq;
    arq = fopen("../utils/input_image_example_RGB.txt","r");
    
    ImageRGB* imagergb = create_image_rgb(arq);

    ImageRGB* clahe = clahe_rgb(imagergb,512,512);

    mostrar_imagem_RGB(imagergb);

    mostrar_imagem_RGB(clahe);

    free_image_rgb(clahe);
    free_image_rgb(imagergb);

    return 0;
}