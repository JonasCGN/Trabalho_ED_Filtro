#ifndef TELA_H
#define TELA_H

typedef struct {
    HistoricoRGB *historicorgb;
    HistoricoGray *historicogray;
    HistoricoRandomGray *histoRandomGray;
    HistoricoRandomRGB *histoRandomRGB;
    GtkWidget *image_widget_gray;
    GtkWidget *image_widget_rgb;
    GtkWidget *image_widget_random;
    ImageRGB *imagergb;
    ImageGray *imagegray;
    ImageRGB *imagerandomRGB;
    ImageGray *imagerandomGray;
} Appdata;

GdkPixbuf* image_rgb_to_pixbuf(ImageRGB *img);
GdkPixbuf* image_gray_to_pixbuf(ImageGray *img);

void app_activate(GApplication *app, gpointer user_data);

#endif // TELA_H