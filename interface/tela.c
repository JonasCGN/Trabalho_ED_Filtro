#include <gtk/gtk.h>
#include <stdlib.h>

#include "../functions/image.h"
#include "tela.h"

GdkPixbuf* image_rgb_to_pixbuf(ImageRGB *img) {
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, img->dim.largura, img->dim.altura);
    
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    int n_channels = gdk_pixbuf_get_n_channels(pixbuf);
    
    for (int i = 0; i < img->dim.altura; i++) {
        for (int j = 0; j < img->dim.largura; j++) {
            PixelRGB *src_pixel = &img->pixels[i * img->dim.largura + j];
            guchar *dest_pixel = pixels + i * rowstride + j * n_channels;
            dest_pixel[0] = src_pixel->red;
            dest_pixel[1] = src_pixel->green;
            dest_pixel[2] = src_pixel->blue;
        }
    }
    
    return pixbuf;
}

GdkPixbuf* image_gray_to_pixbuf(ImageGray *img) {
    // Create a new GdkPixbuf
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, img->dim.largura, img->dim.altura);
    
    // Get the pixel array and row stride from the GdkPixbuf
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    int n_channels = gdk_pixbuf_get_n_channels(pixbuf);

    // Copy the pixels from ImageRGB to GdkPixbuf
    for (int i = 0; i < img->dim.altura; i++) {
        for (int j = 0; j < img->dim.largura; j++) {
            PixelGray *src_pixel = &img->pixels[i * img->dim.largura + j];
            guchar *dest_pixel = pixels + i * rowstride + j * n_channels;
            dest_pixel[0] = src_pixel->value;
            dest_pixel[1] = src_pixel->value;
            dest_pixel[2] = src_pixel->value;
        }
    }
    
    return pixbuf;
}

void on_button1_clicked(GtkButton *button, gpointer user_data){
    g_print("Botao 1 Clicado!\n");
}

void on_button2_clicked(GtkButton *button, gpointer user_data){
    g_print("Botao 2 Clicado!\n");

}

void on_button3_clicked(GtkButton *button, gpointer user_data){
    g_print("Botao 3 Clicado!\n");

}

void on_button4_clicked(GtkButton *button, gpointer user_data){
    g_print("Botao 4 Clicado!\n");

}

void on_button5_clicked(GtkButton *button, gpointer user_data){
    g_print("Botao 5 Clicado!\n");

}

void on_button6_clicked(GtkButton *button, gpointer user_data){
    g_print("Botao 6 Clicado!\n");

}

void on_button7_clicked(GtkButton *button, gpointer user_data){
    g_print("Botao 7 Clicado!\n");

}

void on_button8_clicked(GtkButton *button, gpointer user_data){
    g_print("Botao 8 Clicado!\n");

}

void on_button9_clicked(GtkButton *button, gpointer user_data){
    g_print("Botao 9 Clicado!\n");

}

void on_button10_clicked(GtkButton *button, gpointer user_data){
    g_print("Botao 10 Clicado!\n");

}

void app_activate(GApplication *app, gpointer user_data){
    GtkWidget *window;

    window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(box), left_box, TRUE, TRUE, 0);

    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(box), right_box, FALSE, FALSE, 0);

    GtkWidget *functionGray = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(right_box), functionGray, TRUE, TRUE, 0);

    GtkWidget *functionRGB = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(right_box), functionRGB, TRUE, TRUE, 0);

    GtkWidget *label = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Filtros Disponiveis",
        NULL
    );

    GtkWidget *button1 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Flip Vertical Gray",
        NULL
    ); 

    GtkWidget *button2 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Flip horizontal Gray",
        NULL
    ); 

    GtkWidget *button3 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Transpose Gray",
        NULL
    ); 

    GtkWidget *button4 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Media Blur Gray",
        NULL
    ); 

    GtkWidget *button5 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Clahe Gray",
        NULL
    ); 

    GtkWidget *button6 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Flip Vertical RGB",
        NULL
    ); 

    GtkWidget *button7 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Flip horizontal RGB",
        NULL
    ); 

    GtkWidget *button8 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Transpose RGB",
        NULL
    ); 

    GtkWidget *button9 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Media Blur RGB",
        NULL
    ); 

    GtkWidget *button10 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Clahe RGB",
        NULL
    ); 

    gtk_container_add(GTK_CONTAINER(right_box),button1);
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), NULL);
    // g_object_set_data(G_OBJECT(button1), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(right_box),button2);
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button2_clicked), NULL);
    // g_object_set_data(G_OBJECT(button2), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(right_box),button3);
    g_signal_connect(button3, "clicked", G_CALLBACK(on_button3_clicked), NULL);
    // g_object_set_data(G_OBJECT(button3), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(right_box),button4);
    g_signal_connect(button4, "clicked", G_CALLBACK(on_button4_clicked), NULL);
    // g_object_set_data(G_OBJECT(button4), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(right_box),button5);
    g_signal_connect(button5, "clicked", G_CALLBACK(on_button5_clicked), NULL);
    // g_object_set_data(G_OBJECT(button5), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(right_box),button6);
    g_signal_connect(button6, "clicked", G_CALLBACK(on_button6_clicked), NULL);
    // g_object_set_data(G_OBJECT(button6), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(right_box),button7);
    g_signal_connect(button7, "clicked", G_CALLBACK(on_button7_clicked), NULL);
    // g_object_set_data(G_OBJECT(button7), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(right_box),button8);
    g_signal_connect(button8, "clicked", G_CALLBACK(on_button8_clicked), NULL);
    // g_object_set_data(G_OBJECT(button8), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(right_box),button9);
    g_signal_connect(button9, "clicked", G_CALLBACK(on_button9_clicked), NULL);
    // g_object_set_data(G_OBJECT(button9), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(right_box),button10);
    g_signal_connect(button10, "clicked", G_CALLBACK(on_button10_clicked), NULL);
    // g_object_set_data(G_OBJECT(button10), "image_widget", image);

    gtk_widget_show_all(window);
}