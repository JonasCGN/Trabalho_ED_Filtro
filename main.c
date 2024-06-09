#include <stdio.h>
#include <stdlib.h>

#include "./functions/image.c"

int main(){
    FILE *arq;
    arq = fopen("../utils/input_image_example_RGB.txt","r");
    
    ImageRGB* imagergb = create_image_rgb(arq);

    ImageRGB* clahe = clahe_rgb(imagergb,128,128);

    clahe = median_blur_rgb(clahe,3);

    arq = fopen("../criarImagem/imagemCLahe.txt","w");
    criaTXTImagemRGB(arq,clahe);

    // mostrar_imagem_RGB(imagergb);
    // mostrar_imagem_RGB(clahe);

    free_image_rgb(clahe);
    free_image_rgb(imagergb);

    return 0;
}