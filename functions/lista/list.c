#include <stdio.h>
#include <stdlib.h>

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
            remove = l;

            while (aux->prox != l){
                aux = aux->prox;
            }

            aux->prox = l->prox;        
            l->prox->ant = aux;

            l = remove->prox;

            if(remove == l){
                free(remove);
                l = NULL;
            }        

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
            remove = l;

            while (aux->prox != l){
                aux = aux->prox;
            }

            aux->prox = l->prox;        
            l->prox->ant = aux;

            l = remove->prox;

            if(remove == l){
                free(remove);
                l = NULL;
            }        

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
