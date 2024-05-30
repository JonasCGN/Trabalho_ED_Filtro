#include <stdio.h>
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
}