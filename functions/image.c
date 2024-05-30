#include <stdio.h>
<<<<<<< HEAD
#include <stdlib.h>
#include "image.h"

ImageGray *flip_vertical_gray(ImageGray *image)
{
    ImageGray *new_image = create_image_gray(image->dim.altura, image->dim.largura);
    int altura = image->dim.altura;
    int largura = image->dim.largura;

    for (altura = image->dim.altura - 1; altura >= 0; altura--)
    {
        for (largura = 0; largura < image->dim.largura; largura++)
        {
            new_image->pixels[(image->dim.altura - 1 - altura) * image->dim.largura + largura].value = image->pixels[altura * image->dim.largura + largura].value;
        }
    }
    

    return new_image;
}

ImageGray *flip_horizontal_gray(ImageGray *image)
{
    ImageGray *new_image = create_image_gray(image->dim.altura, image->dim.largura);
    int altura;
    int largura ;

    for (altura = 0; altura < image->dim.altura; altura++)
    {
        for (largura = image->dim.largura - 1; largura >= 0; largura--)
        {
            new_image->pixels[altura * image->dim.largura + (image->dim.largura - 1 - largura)].value = image->pixels[altura * image->dim.largura + largura].value;
        }
    }

    return new_image;
}
void printImage(ImageGray *image)
{
    for (int i = 0; i < image->dim.largura; i++)
    {
        for (int j = 0; j < image->dim.altura; j++)
        {
            printPixel(i, j, image);
        }
        printf("\n");
    }
}
void free_image_gray(ImageGray *image)
{
    free(image->pixels);
    free(image);
=======
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
>>>>>>> 30b52628486873ade7498e3499558a4b6c9c161d
}