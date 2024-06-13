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
    if(image_gray == NULL){
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
    for(int i=0;i<img->dim.altura; i++){
        for (int j= 0; j <img->dim.largura; j++){
            printf("\033[48;2;%d;%d;%dm  \033[0m", img->pixels[i * img->dim.largura + j].value, img->pixels[i * img->dim.largura + j].value, img->pixels[i * img->dim.largura + j].value);
        }
        printf("\n");
    }
}

ImageRGB *create_image_rgb(FILE *file)
{

    int i = 0;
    ImageRGB *image_rgb = (ImageRGB *)malloc(sizeof(ImageRGB));
    if(image_rgb== NULL){
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
    for(int i=0;i<img->dim.altura; i++){
        for (int j= 0; j <img->dim.largura; j++){
            printf("\033[48;2;%d;%d;%dm  \033[0m", img->pixels[i * img->dim.largura + j].red, img->pixels[i * img->dim.largura + j].green, img->pixels[i * img->dim.largura + j].blue);
        }
        printf("\n");
    }
}

void free_image_rgb(ImageRGB *image){
    free(image->pixels);
    free(image);
}

ImageGray *flip_vertical_gray(ImageGray *image){
    ImageGray *new_image = (ImageGray*)malloc(sizeof(ImageGray));
    int altura = image->dim.altura;
    int largura = image->dim.largura;

    new_image->dim.altura = altura;
    new_image->dim.largura = largura;

    new_image->pixels = (PixelGray*)calloc(sizeof(PixelGray), altura * largura);

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
    
    ImageGray *new_image = (ImageGray*)malloc(sizeof(ImageGray));

    int altura = image->dim.altura;
    int largura = image->dim.largura;

    new_image->dim.altura = altura;
    new_image->dim.largura = largura;

    new_image->pixels = (PixelGray*)calloc(sizeof(PixelGray), altura * largura);

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

    int found=0,cdA;
    for(int i=1;i < COR;i++){
        if(!found){
            if(pdf[i] != 0){
                cdf[i] = pdf[i];
                cdA = i;
                found = 1;
            }
        }else{
            if(pdf[i] != 0){
                cdf[i] = cdf[cdA] + pdf[i];
                cdA = i;
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

void interpola(){
    
}

ImageRGB *clahe_rgb(const ImageRGB *image, int tile_width, int tile_height){
    ImageRGB* image_clahe = (ImageRGB*)malloc(sizeof(ImageRGB));
    
    int hR[COR],hG[COR],hB[COR];
    int *hvR,*hvG,*hvB;
    int percorrerX,percorrerY;
    int total_pixels = tile_height * tile_width;

    image_clahe->dim.altura = image->dim.altura;
    image_clahe->dim.largura = image->dim.largura;

    image_clahe->pixels = (PixelRGB*)calloc(sizeof(PixelRGB),image->dim.altura*image->dim.largura);
    
    for(int i=0;i < image->dim.altura * image->dim.largura;i++){
        image_clahe->pixels[i].red = image_clahe->pixels[i].red;
        image_clahe->pixels[i].green = image_clahe->pixels[i].green;
        image_clahe->pixels[i].blue = image_clahe->pixels[i].blue;
    }

    for(int i=0;i < image->dim.altura;i += tile_height){
        for(int j=0;j < image->dim.largura;j += tile_width){
            percorrerX = i + tile_height;
            percorrerY = j + tile_width;

            if(percorrerX > image->dim.altura){
                percorrerX -= (percorrerX - image->dim.altura);
            }
            if(percorrerY > image->dim.largura){
                percorrerY -= (percorrerY - image->dim.largura);
            }

            for(int k=0;k<COR;k++) hR[k] = hG[k] = hB[k] = 0;

            for(int m = i;m < percorrerX;m++){
                for(int n = j;n < percorrerY;n++){
                    int vR = getPixelRGB(image,m,n).red;
                    int vG = getPixelRGB(image,m,n).green;
                    int vB = getPixelRGB(image,m,n).blue;
                    
                    hR[vR]++;
                    hG[vG]++;
                    hB[vB]++;
                }

            }
                
            hvR = calculaHv(hR,total_pixels);
            hvG = calculaHv(hG,total_pixels);
            hvB = calculaHv(hB,total_pixels);

            for(int c=0;c<COR;c++){
                for(int m = i;m < percorrerX;m++)
                    for(int n = j;n < percorrerY;n++){
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
    
    free(hvR);
    free(hvG);
    free(hvB);

    return image_clahe;
}
    
int encontrar_mediana(int *a, int n){
    qsort(a, n, sizeof(int), compare);

    return a[n/2];
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

HistoricoRGB *criaListaRGB(){
    return NULL;
}

HistoricoGray *criaListaGray(){
    return NULL;
}

HistoricoRGB *addFinalDuplamenteCircularRGB(HistoricoRGB *l,ImageRGB *image){
    HistoricoRGB *novo = (HistoricoRGB*)malloc(sizeof(HistoricoRGB));
    HistoricoRGB *aux;

    if(novo){
        novo->imageRGB = image;

        if(l == NULL ){
            novo->prox = novo;
            novo->ant = novo;
            l = novo;

        }else{
            aux = l;

            do{
                aux = aux->prox;
            }while(aux->prox != l);

            novo->ant = aux;
            aux->prox = novo;
            l->ant = novo;
            novo->prox = l;

        }
    
    }else{
        printf("Erro de alocação de memória \n");
    }

    return l;
}

HistoricoGray *addFinalDuplamenteCircularGray(HistoricoGray *l,ImageGray *image){
    HistoricoGray *novo = (HistoricoGray*)malloc(sizeof(HistoricoGray));
    HistoricoGray *aux;

    if(novo){
        novo->imageGray = image;

        if(l == NULL ){
            novo->prox = novo;
            novo->ant = novo;
            l = novo;

        }else{
            aux = l;

            do{
                aux = aux->prox;
            }while(aux->prox != l);

            novo->ant = aux;
            aux->prox = novo;
            l->ant = novo;
            novo->prox = l;

        }
    
    }else{
        printf("Erro de alocação de memória \n");
    }

    return l;
}

void mostrarListaRGB(HistoricoRGB *l){
    HistoricoRGB* aux = l;
    if(l != NULL){
        while (aux->prox != l){
            mostrar_imagem_RGB(aux->imageRGB);
            aux = aux->prox;
        }
        mostrar_imagem_RGB(aux->imageRGB);
        printf("\n");
    }
    else
        printf("Lista vazia");
}

void mostrarListaGray(HistoricoGray *l){
    HistoricoGray* aux = l;
    if(l != NULL){
        while (aux->prox != l){
            mostrar_imagem_Gray(aux->imageGray);
            aux = aux->prox;
        }
        mostrar_imagem_Gray(aux->imageGray);
        printf("\n");    
    }
    else
        printf("Lista vazia");
}

void liberaListaRGB(HistoricoRGB *l){
    HistoricoRGB *aux = l;
    if (l != NULL){
        while (aux->prox != l){
            aux = aux->prox;
            free(aux->ant);
        }
        free(aux);
    }
}

void liberaListaGray(HistoricoGray *l){
    HistoricoGray *aux = l;
    if (l != NULL){
        while (aux->prox != l){
            aux = aux->prox;
            free(aux->ant);
        }
        free(aux);
    }
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
    int incremento = excesso / numB  ;// calcula o incrmeento para distribuir uniformimente 

    for (int j = 0; j < numB; j++){
        if(histo[j] !=0){// branco não entra nisso 
        histo[j] += incremento ; // distribui o execesso entre os outros bins
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
