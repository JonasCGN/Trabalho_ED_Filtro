#include <stdio.h>
#include <stdlib.h>

#include "./functions/image.c"

int main(){
    
    HistoricoRGB* historicoRGB = criaListaRGB();
    HistoricoGray* historicoGray = criaListaGray();
    
    FILE *arq;
    arq = fopen("../utils/input_image_example_RGB.txt","r");
    
    ImageRGB* imagergb = create_image_rgb(arq);
    historicoRGB = addFinalDuplamenteCircularRGB(historicoRGB,imagergb);

    ImageRGB* clahe = clahe_rgb(imagergb,128,128);
    historicoRGB = addFinalDuplamenteCircularRGB(historicoRGB,clahe);

    clahe = median_blur_rgb(clahe,3);
    historicoRGB = addFinalDuplamenteCircularRGB(historicoRGB,clahe);

    arq = fopen("../criarImagem/imagemCLahe.txt","w");
    criaTXTImagemRGB(arq,clahe);

    mostrarListaRGB(historicoRGB);
    mostrarListaGray(historicoGray);

    // mostrar_imagem_RGB(imagergb);
    // mostrar_imagem_RGB(clahe);

    free_image_rgb(clahe);
    free_image_rgb(imagergb);

    if(historicoGray != NULL)
        liberaListaGray(historicoGray);
    if(historicoRGB != NULL)
        liberaListaRGB(historicoRGB);

    return 0;
}