#include <stdio.h>
#include <stdint.h>

#include "image.h"

ImageGray *create_image_gray(FILE *file){
    int i = 0;
    ImageGray *image_gray = (ImageGray *)malloc(sizeof(ImageGray));
    if(image_gray== NULL){
        printf("Erro de alocação da imagem gray!!");
        fclose(file);
        return NULL;
    }
   
    fscanf(file,"%d", &image_gray->dim.altura);
    fscanf(file,"%d", &(image_gray->dim.largura));

    image_gray->pixels = (PixelGray *)calloc(image_gray->dim.altura * image_gray->dim.largura, sizeof(PixelGray));
    if(image_gray->pixels == NULL){
        printf("Erro de alocação de pixel gray \n");
        fclose(file);
        return NULL;
    }
    
    while (!(feof(file))){
        fscanf(file,"%d,",&image_gray->pixels[i].value);
        i++;
    }
     
    fclose(file);
    return image_gray;
}

void mostrar_imagem_Gray(ImageGray *img){
    if(img == NULL || img->pixels == NULL){
        printf("Imagem em tons de cinza invalida\n");
        return;
    }
    for(int i=0;i<img->dim.altura; i++){
        for (int j= 0; j <img->dim.largura; j++){
            printf("\033[38;2;%d;%d;%dm**\033[0m", img->pixels[i * img->dim.largura + j].value, img->pixels[i * img->dim.largura + j].value, img->pixels[i * img->dim.largura + j].value);
        }
        printf("\n");
    }
}