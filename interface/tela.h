#ifndef TELA_H
#define TELA_H

typedef struct {
    HistoricoRGB *historicorgb;
    HistoricoGray *historicogray;
    GtkWidget *image_widget_gray;
    GtkWidget *image_widget_rgb;
} Appdata;

GdkPixbuf* image_rgb_to_pixbuf(ImageRGB *img);
GdkPixbuf* image_gray_to_pixbuf(ImageGray *img);

void app_activate(GApplication *app, gpointer user_data);

#endif // TELA_H