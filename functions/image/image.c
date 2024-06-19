#include <stdio.h>
#include <stdlib.h>

#include "image.h"

#define COR 256

PixelRGB getPixelRGB(const ImageRGB *image, int i, int j)
{
    return image->pixels[i * image->dim.largura + j];
}

PixelGray getPixelGray(const ImageGray *image, int i, int j)
{
    return image->pixels[i * image->dim.largura + j];
}

void criaTXTImagemRGB(FILE *arq, ImageRGB *image)
{

    fprintf(arq, "%d\n", image->dim.altura);
    fprintf(arq, "%d\n", image->dim.largura);

    for (int i = 0; i < image->dim.altura; i++)
    {
        for (int j = 0; j < image->dim.largura; j++)
            fprintf(arq, "%d %d %d,", getPixelRGB(image, i, j).red, getPixelRGB(image, i, j).green, getPixelRGB(image, i, j).blue);
        fprintf(arq, "\n");
    }

    printf("Criado com sucesso!");

    fclose(arq);
}

void criaTXTImagemGray(FILE *arq, ImageGray *image)
{
    fprintf(arq, "%d\n", image->dim.altura);
    fprintf(arq, "%d\n", image->dim.largura);

    for (int i = 0; i < image->dim.altura; i++)
    {
        for (int j = 0; j < image->dim.largura; j++)
            fprintf(arq, "%d %d %d,", getPixelGray(image, i, j).value, getPixelGray(image, i, j).value, getPixelGray(image, i, j).value);
        fprintf(arq, "\n");
    }

    fclose(arq);
}

ImageGray *create_image_gray(FILE *file){
    int i = 0;
    ImageGray *image_gray = (ImageGray *)malloc(sizeof(ImageGray));
    if (image_gray == NULL){
        printf("Erro de alocação da imagem gray!!");
        fclose(file);
        return NULL;
    }

    fscanf(file, "%d", &image_gray->dim.largura);
    fscanf(file, "%d", &image_gray->dim.altura);

    image_gray->pixels = (PixelGray *)calloc(image_gray->dim.altura * image_gray->dim.largura, sizeof(PixelGray));
    if (image_gray->pixels == NULL){
        printf("Erro de alocação de pixel gray \n");
        fclose(file);
        return NULL;
    }

    while (!(feof(file))){
        fscanf(file, "%d,", &image_gray->pixels[i].value);
        i++;
    }

    fclose(file);
    return image_gray;
}

void free_image_gray(ImageGray *image)
{
    free(image->pixels);
    free(image);
}

void mostrar_imagem_Gray(ImageGray *img)
{
    if (img == NULL || img->pixels == NULL)
    {
        printf("Imagem em tons de cinza invalida\n");
        return;
    }
    for (int i = 0; i < img->dim.altura; i++)
    {
        for (int j = 0; j < img->dim.largura; j++)
        {
            printf("\033[48;2;%d;%d;%dm  \033[0m", img->pixels[i * img->dim.largura + j].value, img->pixels[i * img->dim.largura + j].value, img->pixels[i * img->dim.largura + j].value);
        }
        printf("\n");
    }
}

ImageRGB *create_image_rgb(FILE *file){

    int i = 0;
    ImageRGB *image_rgb = (ImageRGB *)malloc(sizeof(ImageRGB));
    if (image_rgb == NULL){
        printf("Erro de alocação da imagem rgb!!");
        fclose(file);
        return NULL;
    }

    fscanf(file, "%d", &image_rgb->dim.largura);
    fscanf(file, "%d", &image_rgb->dim.altura);

    image_rgb->pixels = (PixelRGB *)calloc(image_rgb->dim.altura * image_rgb->dim.largura, sizeof(PixelRGB));
    if (image_rgb->pixels == NULL){
        printf("Erro de alocação de pixel rgb \n");
        fclose(file);
        return NULL;
    }

    while (!(feof(file))){
        fscanf(file, "%d %d %d,", &image_rgb->pixels[i].red,&image_rgb->pixels[i].green, &image_rgb->pixels[i].blue);
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
            printf("\033[48;2;%d;%d;%dm  \033[0m", img->pixels[i * img->dim.largura + j].red, img->pixels[i * img->dim.largura + j].green, img->pixels[i * img->dim.largura + j].blue);
        }
        printf("\n");
    }
}

void free_image_rgb(ImageRGB *image)
{
    free(image->pixels);
    free(image);
}

ImageGray *flip_vertical_gray(ImageGray *image){
    ImageGray *new_image = (ImageGray *)malloc(sizeof(ImageGray));
    int altura = image->dim.altura;
    int largura = image->dim.largura;

    new_image->dim.altura = altura;
    new_image->dim.largura = largura;

    new_image->pixels = (PixelGray *)calloc(sizeof(PixelGray), altura * largura);

    for (int i = altura - 1; i >= 0; i--)
        for (int j = 0; j < largura; j++)
            new_image->pixels[((altura - 1 - i) * largura + j)].value = getPixelGray(image,i,j).value;
    
    return new_image;
}

ImageGray *flip_horizontal_gray(ImageGray *image){

    ImageGray *new_image = (ImageGray *)malloc(sizeof(ImageGray));

    int altura = image->dim.altura;
    int largura = image->dim.largura;

    new_image->dim.altura = altura;
    new_image->dim.largura = largura;

    new_image->pixels = (PixelGray *)calloc(sizeof(PixelGray), altura * largura);

    for (int i = 0; i < image->dim.altura; i++){
        for (int j = image->dim.largura - 1; j >= 0; j--){
            new_image->pixels[i * image->dim.largura + (image->dim.largura - 1 - j)].value = getPixelGray(image,i,j).value;
        }
    }

    return new_image;
}

ImageGray *transposeGray(const ImageGray *image){
    ImageGray *imgray = (ImageGray *)malloc(sizeof(ImageGray));
    if (imgray == NULL){
        printf("Erro de alocacao da imagem gray!\n ");
        return NULL;
    }

    imgray->pixels = (PixelGray *)calloc(sizeof(PixelGray), image->dim.altura * image->dim.largura);
    if (imgray->pixels == NULL){
        printf("Erro de alocação de pixel gray!!\n");
        return NULL;
    }

    imgray->dim.altura = image->dim.largura;
    imgray->dim.largura = image->dim.altura;

    for (int i = 0; i < image->dim.altura; i++){
        for (int j = 0; j < image->dim.largura; j++){
            imgray->pixels[j * imgray->dim.largura + i] = getPixelGray(image, i, j);
        }
    }

    return imgray;
}

ImageRGB *flip_horizontal_rgb(const ImageRGB *image){

    ImageRGB *new_image = (ImageRGB *)malloc(sizeof(ImageRGB));
    if (new_image == NULL){
        printf("Erro ao criar imagem RGB no Flip Horizontal\n");
        return NULL;
    }
    new_image->dim.altura = image->dim.altura;
    new_image->dim.largura = image->dim.largura;

    new_image->pixels = (PixelRGB *)calloc(image->dim.altura * image->dim.largura, sizeof(PixelRGB));
    if (new_image->pixels == NULL){
        printf("Erro de alocação de pixel RGB no Flip Horizontal\n");
        return NULL;
    }
    for (int altura = 0; altura < image->dim.altura; altura++)
        for (int largura = image->dim.largura - 1; largura >= 0; largura--){
            new_image->pixels[altura * new_image->dim.largura + (image->dim.largura - 1 - largura)].red = getPixelRGB(image,altura,largura).red;
            new_image->pixels[altura * new_image->dim.largura + (image->dim.largura - 1 - largura)].green = getPixelRGB(image,altura,largura).green;
            new_image->pixels[altura * new_image->dim.largura + (image->dim.largura - 1 - largura)].blue = getPixelRGB(image,altura,largura).blue;
        }

    return new_image;
}

ImageRGB *flip_vertical_rgb(const ImageRGB *image){
    ImageRGB *new_image = (ImageRGB *)malloc(sizeof(ImageRGB));
    if (new_image == NULL){
        printf("Erro ao criar imagem RGB no Flip Vertical\n");
        return NULL;
    }
    new_image->dim.altura = image->dim.altura;
    new_image->dim.largura = image->dim.largura;

    new_image->pixels = (PixelRGB *)calloc(image->dim.altura * image->dim.largura, sizeof(PixelRGB));
    if (new_image->pixels == NULL){
        printf("Erro ao criar imagem RGB no Flip Vertical\n");
        return NULL;
    }

    int i = 0;

    for (int j = image->dim.altura - 1; j >= 0; j--)
        for (int k = 0; k < image->dim.largura; k++){
            new_image->pixels[i].red = getPixelRGB(image,j,k).red;
            new_image->pixels[i].green = getPixelRGB(image,j,k).green;
            new_image->pixels[i].blue = getPixelRGB(image,j,k).blue;
            i++;
        }

    return new_image;
}

ImageRGB *transposeRGB(const ImageRGB *image){
    ImageRGB *imgRGB = (ImageRGB *)malloc(sizeof(ImageRGB));
    if (!imgRGB){
        printf("Erro ao alocar para imagem transpose!");
    }
    imgRGB->pixels = (PixelRGB *)calloc(sizeof(PixelRGB), image->dim.altura * image->dim.largura);
    if (!imgRGB->pixels){
        printf("Erro ao alocar pixels para imagem transpose!");
    }

    imgRGB->dim.altura = image->dim.largura;
    imgRGB->dim.largura = image->dim.altura;

    for (int i = 0; i < image->dim.altura; i++)
        for (int j = 0; j < image->dim.largura; j++)
            imgRGB->pixels[j * imgRGB->dim.largura + i] = getPixelRGB(image, i, j);

    return imgRGB;
}

void trocar( int *a, int *b){
    int aux = *a;
    *a = *b;
    *b = aux;
}

int encontrar_mediana(int *a, int n){
    int esquerda = 0 , direita = n- 1;
    int k = n/2;

    while (esquerda <= direita){
        // escolhe um pivo aleatorio e coloca no lugar certo
        int indicePivo = esquerda + rand() % (direita - esquerda + 1);
        int valorpivo = a[indicePivo];
        trocar(&a[indicePivo], &a[direita]);

        // partiona o array ao redor do pivo 
        int i = esquerda;
        for (int j = esquerda; j < direita; j++){
            if (a[j] < valorpivo){
                trocar(&a[i], &a[j]);
                i++;
            }
        }
        trocar(&a[i], &a[direita]);

        // verifica se o pivo esta na posição da mediana
        if(i ==k){
            return a[i];
        }else if (i < k){
            esquerda = i + 1;
        }else{
            direita = i - 1;
        }
        
    }
    return -1;// não deve rolar essa porra 
}

PixelGray calcular_mediana_gray(const ImageGray * imagem, int linha, int coluna, int tamanho){
    int metade = tamanho / 2;
    int value[tamanho * tamanho];
    int sup = 0;

    // percorre a janela envolta o pixel 
    for (int i = linha - metade; i <= linha + metade; i++){
       for (int j = coluna - metade; j <= coluna +metade ; j++){
        // verificar os limites 
        int hi= i;
        int hj = j;
        
        if( hi < 0 ) hi = 0;
        if(hi >= imagem->dim.altura) hi = imagem->dim.altura -1;
        if( hj < 0 ) hj = 0;
        if(hj >= imagem->dim.largura) hj = imagem->dim.largura -1;
        
        // armazena o valor das cores na posição (hj e hi)
        value[sup] = imagem->pixels[ hi * imagem->dim.largura +hj].value;
        sup++;
       }
    }
    // calcula a mediana e retorna ela 
    PixelGray mediana;
    mediana.value = encontrar_mediana(value, tamanho * tamanho);

    return mediana;
    
}

ImageGray *median_blur_gray(const ImageGray *image, int kernel_size)
{
    ImageGray *imgBlurGray = (ImageGray *)malloc(sizeof(ImageGray));
    if (!imgBlurGray)
    {
        printf("Erro ao alocar imagem pra median blur!");
    }

    imgBlurGray->pixels = (PixelGray *)calloc(sizeof(PixelGray), image->dim.altura * image->dim.largura);
    if (!imgBlurGray->pixels)
    {
        printf("Erro ao alocar pixels imagem pra median blur!");
    }

    imgBlurGray->dim.altura = image->dim.altura;
    imgBlurGray->dim.largura = image->dim.largura;
    for (int i = 0; i < image->dim.altura * image->dim.largura; i++)
        imgBlurGray->pixels[i].value = image->pixels[i].value;

    if (kernel_size % 2 != 1)
    {
        printf("Erro, não é possivel aplicar o median_blur");
        return imgBlurGray;
    }

    for(int i=0;i < image->dim.altura; i++){
        for(int j=0;j < image->dim.largura; j++){
            imgBlurGray->pixels[i * image->dim.largura + j] = calcular_mediana_gray(image,i,j,kernel_size);
        }
    }

    return imgBlurGray;
}

int *calculaHv(int *histograma, int total_pixel)
{
    float pdf[COR] = {0};

    for (int i = 0; i < COR; i++)
    {
        pdf[i] = histograma[i] / (float)total_pixel;
    }

    float *cdf = (float *)calloc(sizeof(float), COR);

    int found = 0, cdA;
    for (int i = 1; i < COR; i++)
    {
        if (!found)
        {
            if (pdf[i] != 0)
            {
                cdf[i] = pdf[i];
                cdA = i;
                found = 1;
            }
        }
        else
        {
            if (pdf[i] != 0)
            {
                cdf[i] = cdf[cdA] + pdf[i];
                cdA = i;
            }
        }
    }

    for (int i = 0; i < COR; i++)
    {
        cdf[i] *= 255;
    }

    int *hv = (int *)calloc(sizeof(int), COR);

    for (int i = 0; i < COR; i++)
    {
        if (cdf[i] != 0)
        {
            hv[i] = (int)(cdf[i] + 0.5);
        }
    }

    free(cdf);

    return hv;
}

void interpola()
{
}

ImageRGB *clahe_rgb(const ImageRGB *image, int tile_width, int tile_height)
{
    ImageRGB *image_clahe = (ImageRGB *)malloc(sizeof(ImageRGB));

    int hR[COR], hG[COR], hB[COR];
    int *hvR, *hvG, *hvB;
    int percorrerX, percorrerY;
    int total_pixels = tile_height * tile_width;

    image_clahe->dim.altura = image->dim.altura;
    image_clahe->dim.largura = image->dim.largura;

    image_clahe->pixels = (PixelRGB *)calloc(sizeof(PixelRGB), image->dim.altura * image->dim.largura);

    for (int i = 0; i < image->dim.altura * image->dim.largura; i++)
    {
        image_clahe->pixels[i].red = image_clahe->pixels[i].red;
        image_clahe->pixels[i].green = image_clahe->pixels[i].green;
        image_clahe->pixels[i].blue = image_clahe->pixels[i].blue;
    }

    for (int i = 0; i < image->dim.altura; i += tile_height)
    {
        for (int j = 0; j < image->dim.largura; j += tile_width)
        {
            percorrerX = i + tile_height;
            percorrerY = j + tile_width;

            if (percorrerX > image->dim.altura)
            {
                percorrerX -= (percorrerX - image->dim.altura);
            }
            if (percorrerY > image->dim.largura)
            {
                percorrerY -= (percorrerY - image->dim.largura);
            }

            for (int k = 0; k < COR; k++)
                hR[k] = hG[k] = hB[k] = 0;

            for (int m = i; m < percorrerX; m++)
            {
                for (int n = j; n < percorrerY; n++)
                {
                    int vR = getPixelRGB(image, m, n).red;
                    int vG = getPixelRGB(image, m, n).green;
                    int vB = getPixelRGB(image, m, n).blue;

                    hR[vR]++;
                    hG[vG]++;
                    hB[vB]++;
                }
            }

            hvR = calculaHv(hR, total_pixels);
            hvG = calculaHv(hG, total_pixels);
            hvB = calculaHv(hB, total_pixels);

            for (int c = 0; c < COR; c++)
            {
                for (int m = i; m < percorrerX; m++)
                    for (int n = j; n < percorrerY; n++)
                    {
                        if (c == getPixelRGB(image, m, n).red)
                            image_clahe->pixels[m * image->dim.largura + n].red = hvR[c];

                        if (c == getPixelRGB(image, m, n).green)
                            image_clahe->pixels[m * image->dim.largura + n].green = hvG[c];

                        if (c == getPixelRGB(image, m, n).blue)
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

PixelRGB calcular_mediana_rgb(const ImageRGB * imagem, int linha, int coluna, int tamanho){
    int metade = tamanho / 2;
    int vermelho[tamanho * tamanho];
    int verde[tamanho * tamanho];
    int azul[tamanho * tamanho];
    int sup = 0;

    // percorre a janela envolta o pixel 
    for (int i = linha - metade; i <= linha + metade; i++){
       for (int j = coluna - metade; j <= coluna +metade ; j++){
        // verificar os limites 
        int hi= i;
        int hj = j;
        
        if( hi < 0 ) hi = 0;
        if(hi >= imagem->dim.altura) hi = imagem->dim.altura -1;
        if( hj < 0 ) hj = 0;
        if(hj >= imagem->dim.largura) hj = imagem->dim.largura -1;
        
        // armazena o valor das cores na posição (hj e hi)
        vermelho[sup] = imagem->pixels[ hi * imagem->dim.largura +hj].red;
        verde[sup] = imagem->pixels[ hi * imagem->dim.largura +hj].green;
        azul[sup] = imagem->pixels[ hi * imagem->dim.largura +hj].blue;
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

ImageRGB *median_blur_rgb(const ImageRGB *image, int kernel_size)
{
    ImageRGB *imgrgblur = (ImageRGB *)malloc(sizeof(ImageRGB));
    imgrgblur->pixels = (PixelRGB *)calloc(sizeof(PixelRGB), image->dim.altura * image->dim.largura);

    imgrgblur->dim.altura = image->dim.altura;
    imgrgblur->dim.largura = image->dim.largura;

    for(int i=0;i< image->dim.altura * image->dim.largura;i++){
        imgrgblur->pixels[i].red = image->pixels[i].red;
        imgrgblur->pixels[i].green = image->pixels[i].green;
        imgrgblur->pixels[i].blue = image->pixels[i].blue;
    }

    if(kernel_size % 2 != 1){
        printf("Erro não é possivel aplicar filto blur!!\n");
        return imgrgblur;
    }
    // percorre os pixels da imagem
     
    for (int i = 0; i < image->dim.altura ; i++){
       for (int j = 0; j < image->dim.largura ; j++){
        // calcula o valor do pixel 
        PixelRGB pixelmedia = calcular_mediana_rgb(image, i, j, kernel_size);
        // atribui ao novo pixel 
        imgrgblur->pixels[i * image->dim.largura +j] = pixelmedia;
       }
       
    }
    return imgrgblur;
}

void calcula_histograma(const PixelGray *pixels, int largura, int altura, int largtotal, int histograma[], int nunB)
{
    for (int i = 0; i < nunB; i++)
    {
        histograma[i] = 0; // inicializa histograma com 0
    }

    for (int x= 0; x < altura; x++){
        for (int y = 0; y < largura; y++){
            int pixel = pixels[x * largtotal + y].value;// obtem valor do pixel
            if ( pixel < nunB){// garantir que o valor do pixel esteja no intervalo 
                histograma[pixel]++;
            }else{
                histograma[nunB - 1]++;
            }
            
        }
    }
}

void limite_histograma(int histo[], int limite, int numB)
{
    int excesso = 0;

    for (int i = 0; i < numB; i++)
    {
        if (histo[i] > limite)
        {
            excesso += histo[i] - limite; // calcula o excesso de contagem no histograma
            histo[i] = limite;            // define o limite com novo valor para o bin
        }
    }
    if(numB > 0){
        int incremento = excesso / numB ;// calcula o incrmeento para distribuir uniformimente 

        for (int j = 0; j < numB; j++){
           if(histo[j] !=0){// branco não entra nisso 
            histo[j] += incremento; // distribui o execesso entre os outros bins
            }
        }
    }
}

void aplicar_por_bloco(const PixelGray *pixelentrada, PixelGray *pixelsaida, int altura, int largura, int largtotal, int histograma[], int nunB){
    int *cdf = (int *)malloc(nunB * sizeof(int));
    cdf[0] = histograma[0];

    for (int i = 1; i < nunB; i++)
    {
        cdf[i] = cdf[i - 1] + histograma[i]; // calcula o cdf acumulativo
    }

    int numpixel = largura * altura;
    int mincdf = 0;

    for (int j = 0; j < nunB; j++)
    {
        if (cdf[j] > 0)
        {
            mincdf = cdf[j]; // valor minimo do cdf que não seja 0
            break;
        }
    }
    for (int x = 0; x < altura; x++){
       for (int y = 0; y < largura; y++){
        int pixel = pixelentrada[x * largtotal + y].value;// pixel de entrada
            if (pixel < nunB){
                int novopixel = (int)(((float)(cdf[pixel] - mincdf)/ (numpixel - mincdf)) * (nunB -1));// novo valor do pixel
                pixelsaida[x * largtotal + y].value = novopixel;// define o novo valor do pixel de saida
            }else{
                pixelsaida[x * largtotal + y].value = nunB -1;
            }
        }
    } 
  free(cdf);
}

ImageGray *clahe_gray(const ImageGray *image, int tile_width, int tile_height){
    ImageGray *imgclahe = (ImageGray*)malloc(sizeof(ImageGray));
    imgclahe->pixels = (PixelGray *)calloc(image->dim.altura * image->dim.largura, sizeof(PixelGray));

    imgclahe->dim.altura = image->dim.altura;
    imgclahe->dim.largura = image->dim.largura;

    int nunB = COR;
    int limite = 1;// limite de clipagem

    // intera sobre cada bloco 
    for (int i = 0; i < image->dim.altura; i += tile_height ){
        for (int j = 0; j < image->dim.largura; j += tile_width ){

            int altura_atual = (i + tile_height <= image->dim.altura) ? tile_height : (image->dim.altura -  i);
            int largur_atual = (j + tile_width <= image->dim.largura) ? tile_width : (image->dim.largura - j);
           
            // obtem bloco atual 
            const PixelGray *blocoatual = &image->pixels[i * image->dim.largura + j];
            PixelGray *blocoresultado = &imgclahe->pixels[i * image->dim.largura + j];

            int *histograma = (int *) malloc(nunB * sizeof(int));
            calcula_histograma(blocoatual,largur_atual, altura_atual, image->dim.largura, histograma,nunB);
            limite_histograma(histograma,limite, nunB );
            aplicar_por_bloco(blocoatual,blocoresultado, altura_atual,largur_atual, image->dim.largura, histograma, nunB);
        }
    }

    return imgclahe;
}

