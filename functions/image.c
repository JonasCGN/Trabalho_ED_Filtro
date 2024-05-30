#include <stdio.h>
#include <stdlib.h>

#include "image.h"

PixelRGB getPixelRGB(const ImageRGB* image,int i,int j){
    return image->pixels[i * image->dim.largura + j];
}

PixelGray getPixelGray(const ImageGray* image,int i,int j){
    return image->pixels[i * image->dim.largura + j];
}

void free_image_gray(ImageGray *image){
    free(image->pixels);
    free(image);
}

ImageRGB *transposeRGB(const ImageRGB *image){
    ImageRGB *imgRGB = (ImageRGB*)malloc(sizeof(ImageRGB));
    imgRGB->pixels = (PixelRGB*)calloc(sizeof(PixelRGB), image->dim.altura * image->dim.largura);

    imgRGB->dim = image->dim;

    for(int i=0;i<image->dim.altura;i++)
        for(int j=0;j<image->dim.largura;j++)
            imgRGB->pixels[i * image->dim.largura + j] = getPixelRGB(image,j,i);

    return imgRGB;
}

int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int med(const ImageGray *image,int p1x,int p1y,int k_s){
    int tam = k_s * k_s, vet[tam], ker=k_s/2,k=0;

    int cmp1 = p1x + ker,cmp2 = p1y + ker;
    int cmp_1 = p1x - ker,cmp_2 = p1y - ker;
    
    for(int i = cmp1; i >= cmp_1;i--){
        for(int j = cmp2;j >= cmp_2;j--){
            vet[k] = getPixelGray(image,i,j).value; 
            k++;
        }
    }

    qsort(vet, tam, sizeof(int), compare);

    return vet[tam/2];
}

ImageGray *median_blur_gray(const ImageGray *image, int kernel_size){
    ImageGray *imgBlurGray = (ImageGray*)malloc(sizeof(ImageGray));
    imgBlurGray->pixels = (PixelGray*)calloc(sizeof(PixelGray), image->dim.altura * image->dim.largura);
    
    imgBlurGray->dim.altura = image->dim.altura;
    imgBlurGray->dim.largura = image->dim.largura;
    for(int i=0;i< image->dim.altura * image->dim.largura ; i++)
        imgBlurGray->pixels[i].value = image->pixels[i].value;
    
    if(kernel_size % 2 != 1){
        printf("Erro, não é possivel aplicar o median_blur");
        return imgBlurGray;
    }

    int ker = kernel_size/2;

    for(int i=0;i < image->dim.altura; i++){
        for(int j=0;j < image->dim.largura; j++){
            if(
                (i >= ker && i < image->dim.altura - ker)  && 
                (j >= ker && j < image->dim.largura - ker)
            ){
                imgBlurGray->pixels[i * image->dim.largura + j].value = med(image,i,j,kernel_size);
            }else{
                continue;
            }
        }
    }

    return imgBlurGray;
}

