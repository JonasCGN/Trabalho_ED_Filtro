#ifndef LISTA_H
#define LISTA_H

typedef struct historicoRGB{
    ImageRGB *imageRGB;

    struct historicoRGB *prox;
    struct historicoRGB *ant;
} HistoricoRGB;

typedef struct historicoGray{
    ImageGray *imageGray;

    struct historicoGray *prox;
    struct historicoGray *ant;
} HistoricoGray;

typedef struct historicoRandomRGB{
    ImageRGB *imageRGB;
    struct historicoRandomRGB *prox;
} HistoricoRandomRGB;

typedef struct historicoRandomGray{
    ImageGray *imageGray;
    struct historicoRandomGray *prox;
} HistoricoRandomGray;

HistoricoRGB *criaListaRGB();
HistoricoGray *criaListaGray();

HistoricoRGB *addFinalDuplamenteCircularRGB(HistoricoRGB *l,ImageRGB *image);
HistoricoGray *addFinalDuplamenteCircularGray(HistoricoGray *l,ImageGray *image);

HistoricoGray *removerElementoGray(HistoricoGray *l, ImageGray *image);
HistoricoRGB *removerElementoRGB(HistoricoRGB *l, ImageRGB *image);

HistoricoRandomRGB *lista_randon_RGB(HistoricoRandomRGB*l, ImageRGB *image);
HistoricoRandomRGB *lista_randon_RGB_remove(HistoricoRandomRGB *l, ImageRGB *image);
HistoricoRandomRGB *random_RGB(ImageRGB *image, int numero_sorteios);

HistoricoRandomGray *lista_randon_Gray(HistoricoRandomGray*l, ImageGray *image);
HistoricoRandomGray *lista_randon_Gray_remove(HistoricoRandomGray *l, ImageGray *image);
ImageGray* random_gray(ImageGray* image,int op);

void tamanhoListaRGB(HistoricoRGB *l);
void tamanhoListaGray(HistoricoGray *l);

void mostrarListaRGB(HistoricoRGB *l);
void mostrarListaGray(HistoricoGray *l);

void liberaListaRGB(HistoricoRGB *l,HistoricoRGB *cabeca);
void liberaListaGray(HistoricoGray *l,HistoricoGray *cabeca);

void liberaListaRandomGray(HistoricoRandomGray *l);
void liberaListaRandomRGB(HistoricoRandomRGB *l);

void liberaImageRGB(ImageRGB *img);
void liberaImageGray(ImageGray *img);

#endif // LISTA_H