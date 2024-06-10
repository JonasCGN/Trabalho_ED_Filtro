#include <stdio.h>
#include <stdlib.h>

#include "./functions/image.c"

int main(){
<<<<<<< HEAD
    
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
=======
    FILE *arq; 
    arq = fopen("../utils/input_image_example_Gray.txt","r");
    if(!arq){
        printf("erro abrir arquivo\n");
    }
    FILE *arq2; 
    arq2 = fopen("../utils/input_image_example_RGB.txt","r");
    if(!arq2){
        printf("erro abrir arquivo\n");
    }
     
    ImageGray *imgclahe = create_image_gray(arq);
    if(imgclahe == NULL){
        printf("erro ao criar imagem cinza\n");
    }
    arq = fopen("../criar/lulut.txt", "w");
    criaTXTImagemGray(arq, imgclahe);
   int tile_width = 128;
   int tile_height = 128;
    ImageGray *imagemclahe = clahe_gray(imgclahe,tile_width,tile_height);
    if(imagemclahe == NULL){
        printf("erro ao criar imagem clahe cinza\n");
    }
    arq = fopen("../criar/lulu.txt", "w");
    criaTXTImagemGray(arq, imagemclahe);
   

    return 0;
   

>>>>>>> 1d1287485fc369e31f7b2c10253f4e99583ab7f4
}