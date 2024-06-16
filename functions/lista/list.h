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

HistoricoRGB *criaListaRGB();
HistoricoGray *criaListaGray();

HistoricoRGB *addFinalDuplamenteCircularRGB(HistoricoRGB *l,ImageRGB *image);
HistoricoGray *addFinalDuplamenteCircularGray(HistoricoGray *l,ImageGray *image);

HistoricoGray *removerElementoGray(HistoricoGray *l, ImageGray *image);
HistoricoRGB *removerElementoRGB(HistoricoRGB *l, ImageRGB *image);

HistoricoRGB *lista_randon_RGB(HistoricoRGB*l, ImageRGB *image);
HistoricoRGB *lista_randon_RGB_remove(HistoricoRGB *l, ImageRGB *image);
HistoricoRGB *randon_RGB(ImageRGB *image, int numero_sorteios);

ImageGray* random_gray(const ImageGray* image);

void tamanhoListaRGB(HistoricoRGB *l);
void tamanhoListaGray(HistoricoGray *l);

void mostrarListaRGB(HistoricoRGB *l);
void mostrarListaGray(HistoricoGray *l);

void liberaListaRGB(HistoricoRGB *l);
void liberaListaGray(HistoricoGray *l);

#endif // LISTA_H