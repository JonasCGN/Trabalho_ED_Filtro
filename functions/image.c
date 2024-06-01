#include <stdio.h>
#include <stdlib.h>
#include "image.h"

#define COR 256

PixelRGB getPixelRGB(const ImageRGB* image,int i,int j){
    return image->pixels[i * image->dim.largura + j];
}

PixelGray getPixelGray(const ImageGray* image,int i,int j){
    return image->pixels[i * image->dim.largura + j];
}

ImageGray *create_image_gray(FILE *file){
    int i = 0;
    ImageGray *image_gray = (ImageGray *)malloc(sizeof(ImageGray));
    if(!image_gray){
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

void free_image_gray(ImageGray *image){
    free(image->pixels);
    free(image);
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

ImageRGB *create_image_rgb(FILE *file){

    int i = 0;
    ImageRGB *image_rgb = (ImageRGB *)malloc(sizeof(ImageRGB));
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

void free_image_rgb(ImageRGB *image){
    free(image->pixels);
    free(image);
}

ImageGray *flip_vertical_gray(ImageGray *image)
{
    FILE *arq;
    arq = fopen("../utils/input_image_example_Gray.txt","r");
    if(!arq){
        printf("Erro ao abrir o arquivo da imagem gray!");
    }
    ImageGray *new_image = create_image_gray(arq);

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
    FILE *arq;
    arq = fopen("../utils/input_image_example_Gray.txt","r");
    if(!arq){
        printf("Erro ao abrir o arquivo da imagem gray!");
    }
    ImageGray *new_image = create_image_gray(arq);

    int altura;
    int largura;

    for (altura = 0; altura < image->dim.altura; altura++)
    {
        for (largura = image->dim.largura - 1; largura >= 0; largura--)
        {
            new_image->pixels[altura * image->dim.largura + (image->dim.largura - 1 - largura)].value = image->pixels[altura * image->dim.largura + largura].value;
        }
    }

    return new_image;
}

ImageGray *transposeGray(const ImageGray *image){
    ImageGray *imgray = (ImageGray*)malloc(sizeof(ImageGray));
    if(imgray == NULL){
        printf("Erro de alocacao da imagem gray!\n ");
        return NULL;
    }

    imgray->pixels = (PixelGray*)calloc(sizeof(PixelGray), image->dim.altura *image->dim.largura);
    if(imgray->pixels == NULL){
        printf("Erro de alocação de pixel gray!!\n");
        return NULL;
    }

    imgray->dim.altura = image->dim.altura;
    imgray->dim.largura = image->dim.largura;
    
    for (int i = 0; i < image->dim.altura; i++){
        for (int j = 0; j < image->dim.largura; j++){
            imgray->pixels[i * image->dim.largura + j] = getPixelGray(image,j,i);
        }
        
    }
    return imgray;
}

ImageRGB *transposeRGB(const ImageRGB *image){
    ImageRGB *imgRGB = (ImageRGB*)malloc(sizeof(ImageRGB));
    if(!imgRGB){
        printf("Erro ao alocar para imagem transpose!");
    }
    imgRGB->pixels = (PixelRGB*)calloc(sizeof(PixelRGB), image->dim.altura * image->dim.largura);
    if(!imgRGB->pixels){
        printf("Erro ao alocar pixels para imagem transpose!");
    }

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
    if(!imgBlurGray){
        printf("Erro ao alocar imagem pra median blur!");
    }
    imgBlurGray->pixels = (PixelGray*)calloc(sizeof(PixelGray), image->dim.altura * image->dim.largura);
    if(!imgBlurGray->pixels){
        printf("Erro ao alocar pixels imagem pra median blur!");
    }
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

int* calculaHv(int* histograma,int total_pixel){
    float pdf[COR] = {0};

    for(int i=0;i < COR;i++){
        pdf[i] = histograma[i] / (float)total_pixel;
    }

    float *cdf = (float*)calloc(sizeof(float), COR);
    int found=0,d;
    for(int i=1;i < COR;i++){
        if(!found){
            if(pdf[i] != 0){
                cdf[i] = pdf[i];
                d = i;
                found = 1;
            }
        }else{
            if(pdf[i] != 0){
                cdf[i] = cdf[d] + pdf[d];
                d = i;
            }
        }
    }

    for(int i=0;i < COR;i++){
        cdf[i] *= 255;
    }

    int *hv = (int*)calloc(sizeof(int),COR);

    for(int i=0;i < COR;i++){
        if(cdf[i] != 0){
            hv[i] = (int)(cdf[i] + 0.5);
        }
    }

    free(cdf);
    return hv;
}

ImageRGB *clahe_rgb(const ImageRGB *image, int tile_width, int tile_height){
    FILE *arq;

    arq = fopen("../utils/input_image_example_RGB.txt","r");
    ImageRGB* image_clahe = create_image_rgb(arq);
    
    int histograma[COR];
    int *hvR,*hvG,*hvB;
    int total_pixels = tile_height * tile_width;

    for(int i=0;i<image->dim.altura;i++){
        for(int j=0;j<image->dim.largura;j++){

            if(i % tile_width == 0 && j % tile_height == 0){
                for(int k=0;k<COR;k++) histograma[k] = 0;

                for(int m = i + tile_height - 1;m >= i;m--)
                    for(int n = j + tile_width - 1;n >= j;n--){
                        int valor = getPixelRGB(image,m,n).red;
                        histograma[valor]++;
                    }

                hvR = calculaHv(histograma,total_pixels);

                for(int k=0;k < COR;k++) histograma[k] = 0;
                for(int m = i + tile_height - 1;m >= i;m--)
                    for(int n = j + tile_width - 1;n >= j;n--){
                        int valor = getPixelRGB(image,m,n).green;
                        histograma[valor]++;
                    }
                
                hvG = calculaHv(histograma,total_pixels);

                for(int k=0;k<COR;k++) histograma[k] = 0;
                for(int m = i + tile_height - 1;m >= i;m--)
                    for(int n = j + tile_width - 1;n >= j;n--){
                        int valor = getPixelRGB(image,m,n).blue;
                        histograma[valor]++;
                    }

                hvB = calculaHv(histograma,total_pixels);

                for(int c=0;c<COR;c++){
                    for(int m = i + tile_height - 1;m >= i;m--){
                        for(int n = j + tile_width - 1;n >= j;n--){

                            if(c == getPixelRGB(image,m,n).red)
                                image_clahe->pixels[m * image->dim.largura + n].red = hvR[c];
                            
                            if(c == getPixelRGB(image,m,n).green)
                                image_clahe->pixels[m * image->dim.largura + n].green = hvG[c];
                            
                            if(c == getPixelRGB(image,m,n).blue)
                                image_clahe->pixels[m * image->dim.largura + n].blue = hvB[c];
                            
                        }
                    }
                }


            }
        }
    }
    
    free(hvR);
    free(hvG);
    free(hvB);
    return image_clahe;
}