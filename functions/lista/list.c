#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../image/image.h"
#include "list.h"

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

HistoricoGray *removerElementoGray(HistoricoGray *l, ImageGray *image){
    HistoricoGray* aux = l,*remove=NULL;

    if(l != NULL){
        if(l->imageGray == image){
            return l;
        }

        while(aux->prox != l){
            if(aux->prox->imageGray == image){
                remove = aux->prox;
                break;
            }
            aux = aux->prox;
        }
        
        if(remove == NULL){
            printf("Elemento nao encontrado!\n");
        }else{
            if(aux->prox->prox != l)
                aux->prox->prox->ant = aux;

            aux->prox = aux->prox->prox;
            
            aux = l;
            while (aux->prox != l){
                aux = aux->prox;
            }
            l->ant = aux;

            free(remove);
        }
    }else
        printf("Lista vazia\n");

    return l;
}

HistoricoRGB *removerElementoRGB(HistoricoRGB *l, ImageRGB *image){
    HistoricoRGB* aux = l,*remove=NULL;

    if(l != NULL){
        if(l->imageRGB == image){
            return l;
        }

        while(aux->prox != l){
            if(aux->prox->imageRGB == image){
                remove = aux->prox;
                break;
            }
            aux = aux->prox;
        }
        
        if(remove == NULL){
            printf("Elemento nao encontrado!\n");
        }else{
            if(aux->prox->prox != l)
                aux->prox->prox->ant = aux;

            aux->prox = aux->prox->prox;
            
            aux = l;
            while (aux->prox != l){
                aux = aux->prox;
            }
            l->ant = aux;

            free(remove);
        }
    }else
        printf("Lista vazia\n");

    return l;
}

HistoricoRandomRGB *lista_randon_RGB(HistoricoRandomRGB* l, ImageRGB *image){
    HistoricoRandomRGB *novo = (HistoricoRandomRGB*)malloc(sizeof(HistoricoRandomRGB));
    HistoricoRandomRGB *aux;  

    if(novo){
        novo->imageRGB = image;
        novo->prox = NULL;

        if(l == NULL){
            l = novo;
        }else{
            aux = l;
            while (aux->prox != NULL){
                aux = aux->prox;
            }
            aux->prox = novo;
        }
    }else{
        printf(" erro de alocação!!\n");
    }

    return l;
}

HistoricoRandomRGB *lista_randon_RGB_remove(HistoricoRandomRGB *l, ImageRGB *image){
    HistoricoRandomRGB *aux = l, *remove = NULL;

    if(l != NULL){

        if(l->imageRGB == image){
            remove = l;
            l = l->prox;
            free(remove);
        }

        while ( aux->prox->imageRGB != NULL && aux->prox->imageRGB != image){
            aux = aux->prox;
        }
        
        if(aux->prox != NULL){
            remove = aux->prox;
            aux->prox = remove->prox;
            free(remove);

        }else{
            printf("Elemento não encontrado");
        }
        
    }
    return l;
}

HistoricoRandomRGB *random_RGB(ImageRGB *image, int numero_sorteios){
    if(numero_sorteios <=0 || image == NULL){
        printf("Inválido!!\n");
        return NULL;
    }
    
    HistoricoRandomRGB *historico = NULL;
    ImageRGB *resultado = image;

    srand(time(NULL));

    historico = lista_randon_RGB(historico, image);

    for (int i = 0; i < numero_sorteios; i++){
        int rando = rand() % 5;
        int valor_blur = (rand() % 31)+1;
        int valor_clahe_width = (rand() % 27)+1;
        int valor_clahe_heigth = (rand() % 27)+1;

        switch (rando){
            case 0:
                resultado = flip_vertical_rgb(resultado);
            break;
            case 1:
                resultado = flip_horizontal_rgb(resultado);// flip_horizontal

            break;
            case 2:
                resultado =  transposeRGB(resultado);//transpose

            break;
            case 3:
                resultado = clahe_rgb(resultado, valor_clahe_width,valor_clahe_heigth);// clah

            break;
            case 4:
                resultado = median_blur_rgb(resultado,valor_blur);//blur

            break;
            
            default:
                break;
        }
        historico = lista_randon_RGB(historico, resultado);
    }
    return historico;
}
HistoricoRandomGray *lista_randon_Gray(HistoricoRandomGray*l, ImageGray *image){
   HistoricoRandomGray *novo = (HistoricoRandomGray*)malloc(sizeof(HistoricoRandomGray));
   HistoricoRandomGray *aux;  

   if(novo){
    novo->imageGray = image;
    novo->prox = NULL;

        if(l == NULL){
            l = novo;
        }else{
            aux = l;
            while (aux->prox != NULL){
                aux = aux->prox;
            }
            aux->prox = novo;
        }

    }else{
    printf(" erro de alocação!!\n");
   }
   return l;
}

HistoricoRandomGray *lista_randon_Gray_remove(HistoricoRandomGray *l, ImageGray *image){
    HistoricoRandomGray *aux = l, *remove = NULL;

    if(l != NULL){

        if(l->imageGray == image){
            remove = l;
            l = l->prox;
            free(remove);
        }

        while ( aux->prox->imageGray != NULL && aux->prox->imageGray != image){
            aux = aux->prox;
        }
        
        if(aux->prox != NULL){
            remove = aux->prox;
            aux->prox = remove->prox;
            free(remove);

        }else{
            printf("Elemento não encontrado");
        }
        
    }
    return l;
}

ImageGray* random_gray(ImageGray* image, int op){
    ImageGray *new_image = (ImageGray *)malloc(sizeof(ImageGray));
    
    if (new_image == NULL)
    {
        printf("Erro de alocação de imagem gray!!\n");
        return NULL;
    }
    srand(time(NULL));
    
    int valor_blur = rand() % 100 + 1;
    int valor_clahe_width = rand() % 100 + 1;
    int valor_clahe_heigth = rand() % 100 + 1;

    switch (op)
    {
    case 0:
        new_image = flip_horizontal_gray(image);
        break;

    case 1:
        new_image = flip_vertical_gray(image);
        break;
    case 2:
        new_image = transposeGray(image);
        break;
    case 3:
        new_image = median_blur_gray(image, valor_blur);
        break;
    case 4:
        new_image = clahe_gray(image, valor_clahe_width, valor_clahe_heigth);
        break;
    }

    return new_image;
}

void tamanhoListaRGB(HistoricoRGB *l){
    int i=0;
    HistoricoRGB* aux = l;
    
    if(l != NULL){
        do{
            i++;
            aux = aux->prox;
        }while (aux != l);
    }
    else
        printf("Lista vazia");
    
    printf("Tamanho lista: %d", i);
}

void tamanhoListaGray(HistoricoGray *l){
    int i=0;
    HistoricoGray* aux = l;
    
    if(l != NULL){
        do{
            i++;
            aux = aux->prox;
        }while (aux != l);
    }
    else
        printf("Lista vazia");

    printf("Tamanho lista: %d", i);
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
        do{
            mostrar_imagem_Gray(aux->imageGray);
            aux = aux->prox;
        }while (aux->prox != l);

        printf("\n");    
    }
    else
        printf("Lista vazia");
}

void liberaListaRGB(HistoricoRGB *l,HistoricoRGB *cabeca){
    if (l->prox == cabeca){
        free(l);
        return;
    }else{
        liberaListaRGB(l->prox,cabeca);
        liberaImageRGB(l->imageRGB);
        free(l);
    }
}

void liberaListaGray(HistoricoGray *l,HistoricoGray *cabeca){
    if (l->prox == cabeca){
        free(l);
        return;
    }else{
        liberaListaGray(l->prox,cabeca);
        liberaImageGray(l->imageGray);
        free(l);
    }
}

void liberaListaRandomGray(HistoricoRandomGray *l){
    if (l == NULL)
        return;
    else{
        liberaListaRandomGray(l->prox);
        liberaImageGray(l->imageGray);
        free(l);
    }
}

void liberaListaRandomRGB(HistoricoRandomRGB *l){
    if (l == NULL)
        return;
    else{
        liberaListaRandomRGB(l->prox);
        liberaImageRGB(l->imageRGB);
        free(l);
    }
}

void liberaImageRGB(ImageRGB *img){
    if(img != NULL){
        free(img->pixels);
        free(img);
    }
}

void liberaImageGray(ImageGray *img){
    if(img != NULL){
        free(img->pixels);
        free(img);
    }
}
