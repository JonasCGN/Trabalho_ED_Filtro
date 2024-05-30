#include <stdio.h>
#include <stdlib.h>
#include "image.h"

ImageGray *flip_vertical_gray(ImageGray *image)
{
    ImageGray *new_image = image;
    if(new_image == NULL){
        printf("Erro ao criar imagem gray\n");
        return NULL;
    }
    int i = 0;
    for(int j = new_image->dim.altura - 1; j >= 0; j--){
        for(int k = 0; k < new_image->dim.largura; k++){
            new_image->pixels[i].value = new_image->pixels[j * new_image->dim.largura + k].value;
            i++;
        }
    }

   
    return new_image;
}

ImageGray *flip_horizontal_gray(ImageGray *image)
{
    ImageGray *new_image = image;
    if(new_image == NULL){
        printf("Erro ao criar imagem gray\n");
        return NULL;
    }
    new_image->dim.altura = image->dim.altura;
    new_image->dim.largura = image->dim.largura;
     for (int altura = 0; altura < image->dim.altura; altura++)
    {
        for (int largura = image->dim.largura - 1; largura >= 0; largura--)
        {
            new_image->pixels[altura * image->dim.largura + (image->dim.largura - 1 - largura)].value = image->pixels[altura * image->dim.largura + largura].value;
        }
    }


  


    return new_image;
}

ImageGray *create_image_gray(FILE *file){
    int i = 0;
    ImageGray *image_gray = (ImageGray *)malloc(sizeof(ImageGray));
    printf("Alocando imagem gray\n");
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
    system("PAUSE");
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

ImageRGB *create_image_rgb(FILE *file){

    int i = 0;
    ImageRGB *image_rgb = (ImageRGB *)malloc(sizeof(ImageRGB));
    printf("Alocando imagem rgb\n");
    if(image_rgb== NULL){
        printf("Erro de alocação da imagem rgb!!");
        fclose(file);
        return NULL;
    }
   
    fscanf(file,"%d", &image_rgb->dim.altura);
    fscanf(file,"%d", &(image_rgb->dim.largura));

    image_rgb->pixels = (PixelRGB *)calloc(image_rgb->dim.altura * image_rgb->dim.largura, sizeof(PixelRGB));
    if(image_rgb->pixels == NULL){
        printf("Erro de alocação de pixel rgb \n");
        fclose(file);
        return NULL;
    }
    
    while (!(feof(file))){
        fscanf(file,"%d,",&image_rgb->pixels[i].red);
        fscanf(file,"%d,",&image_rgb->pixels[i].green);
        fscanf(file,"%d,",&image_rgb->pixels[i].blue);
        i++;
    }
     
    fclose(file);
    return image_rgb;
}

void mostrar_imagem_RGB(ImageRGB *img){
    system("PAUSE");
    if(img == NULL || img->pixels == NULL){
        printf("Imagem em tons de cinza invalida\n");
        return;
    }
    for(int i=0;i<img->dim.altura; i++){
        for (int j= 0; j <img->dim.largura; j++){
            printf("\033[38;2;%d;%d;%dm**\033[0m", img->pixels[i * img->dim.largura + j].red, img->pixels[i * img->dim.largura + j].green, img->pixels[i * img->dim.largura + j].blue);
        }
        printf("\n");
    }
}