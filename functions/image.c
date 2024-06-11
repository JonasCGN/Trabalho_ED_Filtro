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

void criaTXTImagemRGB(FILE *arq,ImageRGB* image){

    fprintf(arq,"%d\n",image->dim.altura);
    fprintf(arq,"%d\n",image->dim.largura);

    for(int i=0;i<image->dim.altura;i++){
        for(int j=0;j < image->dim.largura;j++)
            fprintf(arq,"%d %d %d,", getPixelRGB(image,i,j).red,getPixelRGB(image,i,j).green,getPixelRGB(image,i,j).blue);
        fprintf(arq,"\n");
    }
    
    printf("Criado com sucesso!");

    fclose(arq);
}

void criaTXTImagemGray(FILE *arq,ImageGray* image){
    fprintf(arq,"%d\n",image->dim.altura);
    fprintf(arq,"%d\n",image->dim.largura);

    for(int i=0;i<image->dim.altura;i++){
        for(int j=0;j < image->dim.largura;j++)
            fprintf(arq,"%d %d %d,", getPixelGray(image,i,j).value,getPixelGray(image,i,j).value,getPixelGray(image,i,j).value);
        fprintf(arq,"\n");
    }

    fclose(arq);
}

ImageGray *create_image_gray(FILE *file)
{
    int i = 0;
    ImageGray *image_gray = (ImageGray *)malloc(sizeof(ImageGray));
    if(image_gray== NULL){
        printf("Erro de alocação da imagem gray!!");
        fclose(file);
        return NULL;
    }

    fscanf(file, "%d", &image_gray->dim.altura);
    fscanf(file, "%d", &(image_gray->dim.largura));

    image_gray->pixels = (PixelGray *)calloc(image_gray->dim.altura * image_gray->dim.largura, sizeof(PixelGray));
    if (image_gray->pixels == NULL)
    {
        printf("Erro de alocação de pixel gray \n");
        fclose(file);
        return NULL;
    }

    while (!(feof(file)))
    {
        fscanf(file, "%d,", &image_gray->pixels[i].value);
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
    for (int i = 0; i < img->dim.altura; i++)
    {
        for (int j = 0; j < img->dim.largura; j++)
        {
            printf("\033[38;2;%d;%d;%dm**\033[0m", img->pixels[i * img->dim.largura + j].value, img->pixels[i * img->dim.largura + j].value, img->pixels[i * img->dim.largura + j].value);
        }
        printf("\n");
    }
}

ImageRGB *create_image_rgb(FILE *file)
{

    int i = 0;
    ImageRGB *image_rgb = (ImageRGB *)malloc(sizeof(ImageRGB));
    printf("Alocando imagem rgb\n");
    if (image_rgb == NULL)
    {
        printf("Erro de alocação da imagem rgb!!");
        fclose(file);
        return NULL;
    }

    fscanf(file, "%d", &image_rgb->dim.altura);
    fscanf(file, "%d", &(image_rgb->dim.largura));

    image_rgb->pixels = (PixelRGB *)calloc(image_rgb->dim.altura * image_rgb->dim.largura, sizeof(PixelRGB));
    if (image_rgb->pixels == NULL)
    {
        printf("Erro de alocação de pixel rgb \n");
        fclose(file);
        return NULL;
    }

    while (!(feof(file)))
    {
        fscanf(file, "%d,", &image_rgb->pixels[i].red);
        fscanf(file, "%d,", &image_rgb->pixels[i].green);
        fscanf(file, "%d,", &image_rgb->pixels[i].blue);
        i++;
    }

    fclose(file);
    return image_rgb;
}

void mostrar_imagem_RGB(ImageRGB *img)
{
    system("PAUSE");
    if (img == NULL || img->pixels == NULL)
    {
        printf("Imagem em tons de cinza invalida\n");
        return;
    }
    for (int i = 0; i < img->dim.altura; i++)
    {
        for (int j = 0; j < img->dim.largura; j++)
        {
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
    ImageGray *new_image = create_image_gray(arq);
    fclose(arq);

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
    ImageGray *new_image = create_image_gray(arq);
    fclose(arq);

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

ImageRGB *flip_horizontal_rgb(const ImageRGB *image)
{

    ImageRGB *new_image = (ImageRGB *)malloc(sizeof(ImageRGB));
    if (new_image == NULL)
    {
        printf("Erro ao criar imagem RGB\n");
        return NULL;
    }
    new_image->dim.altura = image->dim.altura;
    new_image->dim.largura = image->dim.largura;

    new_image->pixels = (PixelRGB *)calloc(image->dim.altura * image->dim.largura, sizeof(PixelRGB));
    if (new_image->pixels == NULL)
    {
        printf("Erro de alocação de pixel RGB \n");
        return NULL;
    }
    for (int altura = 0; altura < image->dim.altura; altura++)
    {
        for (int largura = image->dim.largura - 1; largura >= 0; largura--)
        {
            new_image->pixels[altura * new_image->dim.largura + (image->dim.largura - 1 - largura)].red = image->pixels[altura * image->dim.largura + largura].red;
            new_image->pixels[altura * new_image->dim.largura + (image->dim.largura - 1 - largura)].green = image->pixels[altura * image->dim.largura + largura].green;
            new_image->pixels[altura * new_image->dim.largura + (image->dim.largura - 1 - largura)].blue = image->pixels[altura * image->dim.largura + largura].blue;
        }
    }
    return new_image;
}

ImageRGB *flip_vertical_rgb(const ImageRGB *image)
{
    ImageRGB *new_image = (ImageRGB *)malloc(sizeof(ImageRGB));
    if (new_image == NULL)
    {
        printf("Erro ao criar imagem RGB\n");
        return NULL;
    }
    new_image->dim.altura = image->dim.altura;
    new_image->dim.largura = image->dim.largura;

    new_image->pixels = (PixelRGB *)calloc(image->dim.altura * image->dim.largura, sizeof(PixelRGB));
    if (new_image->pixels == NULL)
    {
        printf("Erro ao criar imagem RGB\n");
        return NULL;
    }

    int i = 0;

    for (int j = image->dim.altura - 1; j >= 0; j--)
    {
        for (int k = 0; k < image->dim.largura; k++)
        {
            new_image->pixels[i].red = image->pixels[j * image->dim.largura + k].red;
            new_image->pixels[i].green = image->pixels[j * image->dim.largura + k].green;
            new_image->pixels[i].blue = image->pixels[j * image->dim.largura + k].blue;
            i++;
        }
    }

    return new_image;
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

int encontrar_mediana(int *a, int n){
   // arruma
   for (int i = 0; i < n - 1; i++){
        for (int j = 0; j < n + 1; j++){
            if(a[j] > a[j + 1]){
                int tempo = a[j];
                a[j] = a[j+1];
                a[j +1] = tempo;
            }
        }
   }  
    return a[n/2]; // retonar o valor do meio do vetor ordenado 
}

PixelRGB calcular_mediana(const ImageRGB * imagem, int linha, int coluna, int tamanho){
    int metade = tamanho / 2;
    int vermelho[tamanho * tamanho];
    int verde[tamanho * tamanho];
    int azul[tamanho * tamanho];
    int sup = 0;

    // percorre a janela envolta o pixel 
    for (int i = linha - metade; i <= linha + metade; i++){
       for (int j = coluna - metade; j <= coluna +metade ; j++){
        // armazena o valor das cores na posição i e j 
        vermelho[sup] = imagem->pixels[ i * imagem->dim.largura +j].red;
        verde[sup] = imagem->pixels[ i * imagem->dim.largura +j].green;
        azul[sup] = imagem->pixels[ i * imagem->dim.largura +j].blue;
        sup++;
       }
    }
    // calcula a mediana e retorna ela 
    PixelRGB mediana;
    mediana.red = encontrar_mediana(vermelho, tamanho * tamanho);
    mediana.green = encontrar_mediana(verde, tamanho * tamanho);
    mediana.blue = encontrar_mediana(azul, tamanho * tamanho);

    return mediana;
    
}

ImageRGB *median_blur_rgb(const ImageRGB *image, int kernel_size){
    ImageRGB *imgrgblur = (ImageRGB*)malloc(sizeof(ImageRGB));
    imgrgblur->pixels = (PixelRGB*)calloc(sizeof(PixelRGB), image->dim.altura * image->dim.largura);

    imgrgblur->dim.altura = image->dim.altura;
    imgrgblur->dim.largura = image->dim.largura;

    if(kernel_size % 2 !=1){
        printf("Erro não é possivel aplicar filtro!!\n");
        return imgrgblur;
    }
    int metade = kernel_size/2;
     
    // percorre os pixels da imagem 
    for (int i = metade; i < image->dim.altura - metade; i++){
       for (int j = metade; j < image->dim.largura - metade ; j++){
        // calcula o valor do pixel 
        PixelRGB pixelmedia = calcular_mediana(image, i, j, kernel_size);
        // atribui ao novo pixel 
        imgrgblur->pixels[i * image->dim.largura +j] = pixelmedia;
       }
       
    }
    return imgrgblur;
}

void calcula_histograma(const PixelGray *pixels, int largura, int altura, int largtotal, int histograma[], int nunB){
    for (int i = 0; i < nunB; i++){
        histograma[i] = 0;// inicializa histograma com 0
    }

    for (int x= 0; x < altura; x++){
       for (int y = 0; y < largura; y++){
        int pixel = pixels[x * largtotal + y].value;// obtem valor do pixel
        histograma[pixel]++;
       }
    }  
}

void limite_histograma(int histo[], int limite, int numB){
    int excesso = 0;

    for (int i = 0; i < numB; i++){
       if (histo[i]>limite){
            excesso += histo[i] - limite;// calcula o excesso de contagem no histograma
            histo[i] = limite; // define o limite com novo valor para o bin
       }
    }
    int incremento = excesso / numB ;// calcula o incrmeento para distribuir uniformimente 

    for (int j = 0; j < numB; j++){
        if(histo[j] !=0){// branco não entra nisso 
        histo[j] += incremento; // distribui o execesso entre os outros bins
        }
    }
}

void aplicar_por_bloco(const PixelGray *pixelentrada, PixelGray *pixelsaida, int altura, int largura, int largtotal, int histograma[], int nunB){
    int cdf[nunB];
    cdf[0] = histograma[0];

    for (int i = 1; i < nunB; i++){
       cdf[i] = cdf[i - 1] + histograma[i];// calcula o cdf acumulativo
    }
    
    int numpixel = largura * altura;
    int mincdf = 0;

    for (int j= 0; j < nunB; j++){
        if(cdf[j] > 0){
            mincdf = cdf[j]; // valor minimo do cdf que não seja 0
            break;
        }
    }
    for (int x = 0; x < altura; x++){
       for (int y = 0; y < largura; y++){
        int pixel = pixelentrada[x * largtotal + y].value;// pixel de entrada
        int novopixel = (int)(((float)(cdf[pixel] - mincdf)/ (numpixel - mincdf)) * (nunB -1));// novo valor do pixel
        pixelsaida[x * largtotal + y].value = novopixel;// define o novo valor do pixel de saida
       }
    } 

}

ImageGray *clahe_gray(const ImageGray *image, int tile_width, int tile_height){
    ImageGray *imgclahe = (ImageGray*)malloc(sizeof(ImageGray));
    imgclahe->pixels = (PixelGray*)calloc(sizeof(PixelGray), image->dim.altura * image->dim.largura);

    imgclahe->dim.altura = image->dim.altura;
    imgclahe->dim.largura = image->dim.largura;
    int nunB = COR;
    int limite = 1;// limite de clipagem
    int largur_atual = 0;
    int altura_atual= 0;

    // intera sobre cada bloco 
    for (int i = 0; i < image->dim.altura; i+=tile_height ){
        for (int j = 0; j < image->dim.altura; j+= tile_width ){
           
            if(i + tile_width > image->dim.largura){
                largur_atual = image->dim.largura-i;
            }else{
                largur_atual = tile_width;
            }// arrumar 
            if(j + tile_height > image->dim.altura){
                altura_atual = image->dim.altura-j;
            }else{
                altura_atual = tile_height;
            }// arrumar 
            // obtem bloco atual 
            const PixelGray *blocoatual = &image->pixels[i * image->dim.largura + j];
            PixelGray *blocoresultado = &imgclahe->pixels[i * image->dim.largura + j];

            int histograma[nunB];
            calcula_histograma(blocoatual,largur_atual, altura_atual, image->dim.largura, histograma,nunB);
            limite_histograma(histograma,limite, nunB );
            aplicar_por_bloco(blocoatual,blocoresultado, largur_atual,altura_atual, image->dim.largura, histograma, nunB);
        }
        
    }
    
   return imgclahe;

}
