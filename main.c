#include <stdio.h>
#include <stdlib.h>

#include "./functions/image.c"


int main(){
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

    
 
   int tile_width = 128;
   int tile_height = 128;
    ImageGray *imagemclahe = clahe_gray(imgclahe,tile_width,tile_height);
    if(imagemclahe == NULL){
        printf("erro ao criar imagem clahe cinza\n");
    }
    arq = fopen("../criar/lulu.txt", "w");
   criaTXTImagemGray(arq, imagemclahe);
   
    
    // mostrar_imagem_RGB(imageblu);
    // free_image_rgb(imageblu);
    // free_image_rgb(imagemrgbblur);
    return 0;
   

}