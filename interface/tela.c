#include <gtk/gtk.h>
#include <stdlib.h>

#include "../functions/image.h"
#include "tela.h"

typedef struct {
    GtkWidget *entry1;
    GtkWidget *entry2;
} DialogData;

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
    Appdata *app_data = (Appdata*)user_data;
    ImageGray *image;

    // addFinalDuplamenteCircularGray(user_data->histoGray,image);

    g_print("Botao 1 Clicado!\n");
}

void on_button2_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata*)user_data;
    ImageGray *image;

    // addFinalDuplamenteCircularGray(user_data->histoGray,image);
    
    g_print("Botao 2 Clicado!\n");
}

void on_button3_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata*)user_data;
    ImageGray *image;

    // addFinalDuplamenteCircularGray(user_data->histoGray,image);
    
    g_print("Botao 3 Clicado!\n");
}

void on_dialog4_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
    if (response_id == GTK_RESPONSE_OK) {
        Appdata *app_data = (Appdata*)user_data;
        ImageGray *image;

        // addFinalDuplamenteCircularGray(user_data->histoGray,image);
        GtkEntry *entry = GTK_ENTRY(user_data);
        const gchar *text = gtk_entry_get_text(entry);
        
        int num = atoi(text);

        g_print("Número inserido: %d\n", num);
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void on_button4_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *entry;
    GtkWindow *parent_window = GTK_WINDOW(user_data);

    // Criar um novo diálogo
    dialog = gtk_dialog_new_with_buttons(
        "Insira um número para a caixa",
        parent_window,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK",
        GTK_RESPONSE_OK,
        "_Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    entry = gtk_entry_new();
    // gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Digite um número");

    // Configurar o campo de entrada para aceitar apenas números
    // gtk_entry_set_input_purpose(GTK_ENTRY(entry), GTK_INPUT_PURPOSE_DIGITS);

    gtk_container_add(GTK_CONTAINER(content_area), entry);

    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog4_response), entry);

    gtk_widget_show_all(dialog);

    g_print("Botao 4 Clicado!\n");
}

void on_dialog5_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
    if (response_id == GTK_RESPONSE_OK) {
        Appdata *app_data = (Appdata*)user_data;
        ImageGray *image;

        // addFinalDuplamenteCircularRGB(user_data->histoGray,image);
        GtkWidget **entries = (GtkWidget **)user_data;

        const gchar *text1 = gtk_entry_get_text(GTK_ENTRY(entries[0]));
        const gchar *text2 = gtk_entry_get_text(GTK_ENTRY(entries[1]));
        
        int num1 = atoi(text1);
        int num2 = atoi(text2);

        g_print("Número 1 inserido: %d\n", num1);
        g_print("Número 2 inserido: %d\n", num2);
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void on_button5_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWindow *parent_window = GTK_WINDOW(user_data);
    GtkWidget *entry1,*entry2;
    DialogData *dialog_data = g_malloc(sizeof(DialogData));

    // Criar um novo diálogo
    dialog = gtk_dialog_new_with_buttons(
        "Insira um número para a caixa",
        parent_window,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK",
        GTK_RESPONSE_OK,
        "_Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    entry1 = gtk_entry_new();
    entry2 = gtk_entry_new();
    dialog_data->entry1 = entry1;
    dialog_data->entry2 = entry2;
    // gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Digite um número");

    // Configurar o campo de entrada para aceitar apenas números
    // gtk_entry_set_input_purpose(GTK_ENTRY(entry), GTK_INPUT_PURPOSE_DIGITS);

    gtk_container_add(GTK_CONTAINER(content_area), entry1);
    gtk_container_add(GTK_CONTAINER(content_area), entry2);

    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog5_response), dialog_data);

    gtk_widget_show_all(dialog);

    g_print("Botao 5 Clicado!\n");
}

void on_button6_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata*)user_data;
    ImageRGB *image;

    // addFinalDuplamenteCircularRGB(user_data->histoRGB,image);

    g_print("Botao 6 Clicado!\n");
}

void on_button7_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata*)user_data;
    ImageRGB *image;

    // addFinalDuplamenteCircularRGB(user_data->histoRGB,image);

    g_print("Botao 7 Clicado!\n");
}

void on_button8_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata*)user_data;
    ImageRGB *image;

    // addFinalDuplamenteCircularRGB(user_data->histoRGB,image);

    g_print("Botao 8 Clicado!\n");
}

void on_dialog9_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
    if (response_id == GTK_RESPONSE_OK) {
        Appdata *app_data = (Appdata*)user_data;
        ImageRGB *image;

        // addFinalDuplamenteCircularRGB(user_data->histoRGB,image);
        GtkEntry *entry = GTK_ENTRY(user_data);
        const gchar *text = gtk_entry_get_text(entry);
        
        int num = atoi(text);

        g_print("Número inserido: %d\n", num);
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void on_button9_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *entry;
    GtkWindow *parent_window = GTK_WINDOW(user_data);

    // Criar um novo diálogo
    dialog = gtk_dialog_new_with_buttons(
        "Insira um número para a caixa",
        parent_window,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK",
        GTK_RESPONSE_OK,
        "_Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    entry = gtk_entry_new();
    // gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Digite um número");

    // Configurar o campo de entrada para aceitar apenas números
    // gtk_entry_set_input_purpose(GTK_ENTRY(entry), GTK_INPUT_PURPOSE_DIGITS);

    gtk_container_add(GTK_CONTAINER(content_area), entry);

    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog9_response), entry);

    gtk_widget_show_all(dialog);

    g_print("Botao 9 Clicado!\n");
}

void on_dialog10_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
    if (response_id == GTK_RESPONSE_OK) {
        Appdata *app_data = (Appdata*)user_data;
        ImageRGB *image;

        // addFinalDuplamenteCircularRGB(user_data->histoRGB,image);
        DialogData *dialog_data = (DialogData *)user_data;

        const gchar *text1 = gtk_entry_get_text(GTK_ENTRY(dialog_data->entry1));
        const gchar *text2 = gtk_entry_get_text(GTK_ENTRY(dialog_data->entry2));
        
        int num1 = atoi(text1);
        int num2 = atoi(text2);

        g_print("Número 1 inserido: %d\n", num1);
        g_print("Número 2 inserido: %d\n", num2);
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void on_button10_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWindow *parent_window = GTK_WINDOW(user_data);
    DialogData *dialog_data = g_malloc(sizeof(DialogData));
    GtkWidget *entry1,*entry2;  // Array to store the entry widgets

    // Criar um novo diálogo
    dialog = gtk_dialog_new_with_buttons(
        "Insira números para as caixas",
        parent_window,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK",
        GTK_RESPONSE_OK,
        "_Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    entry1 = gtk_entry_new();
    entry2 = gtk_entry_new();

    dialog_data->entry1 = entry1;
    dialog_data->entry2 = entry2;

    // Adicionar os campos de entrada ao content_area
    gtk_container_add(GTK_CONTAINER(content_area), entry1);
    gtk_container_add(GTK_CONTAINER(content_area), entry2);

    // Conectar o sinal de resposta do diálogo à função de callback
    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog10_response), dialog_data);

    gtk_widget_show_all(dialog);
    g_print("Botao 10 Clicado!\n");
}

void on_button11_clicked(GtkButton *button, gpointer user_data){
    g_print("Botao 11 Clicado!\n");

}

void on_button12_clicked(GtkButton *button, gpointer user_data){
    g_print("Botao 12 Clicado!\n");

}

void on_button13_clicked(GtkButton *button, gpointer user_data){
    g_print("Botao 13 Clicado!\n");

}

void app_activate(GApplication *app, gpointer user_data){
    GtkWidget *window;
    GtkWidget *image;

    window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    //Cria a Box da Janela
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    //Cria a Box da Imagem1
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(box), left_box, TRUE, TRUE, 10);
    
    //Exibe Imagem Inicial
    Appdata *app_data = (Appdata *)user_data;
    ImageRGB *img = (ImageRGB *)app_data->historicorgb->imageRGB;
    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(img);
    image = gtk_image_new_from_pixbuf(pixbuf);
    gtk_box_pack_start(GTK_BOX(left_box), image, TRUE, TRUE, 0);

    //Cria a box das funções
    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(box), right_box, FALSE, TRUE, 10);

    GtkWidget *titleTop = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Filtros Disponiveis",
        NULL
    );

    gtk_container_add(GTK_CONTAINER(right_box), titleTop);

    GtkWidget *functions = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(right_box), functions, FALSE, FALSE, 0);

    GtkWidget *functionGray = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(functions), functionGray, TRUE, TRUE, 0);

    GtkWidget *functionRGB = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(functions), functionRGB, TRUE, TRUE, 0);

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

    GtkWidget *button11 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "<<",
        NULL
    ); 

    GtkWidget *button12 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "🗑️",
        NULL
    ); 

    GtkWidget *button13 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", ">>",
        NULL
    ); 

    gtk_container_add(GTK_CONTAINER(functionGray),button1);
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), NULL);
    // g_object_set_data(G_OBJECT(button1), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(functionGray),button2);
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button2_clicked), NULL);
    // g_object_set_data(G_OBJECT(button2), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(functionGray),button3);
    g_signal_connect(button3, "clicked", G_CALLBACK(on_button3_clicked), NULL);
    // g_object_set_data(G_OBJECT(button3), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(functionGray),button4);
    g_signal_connect(button4, "clicked", G_CALLBACK(on_button4_clicked), NULL);
    // g_object_set_data(G_OBJECT(button4), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(functionGray),button5);
    g_signal_connect(button5, "clicked", G_CALLBACK(on_button5_clicked), NULL);
    // g_object_set_data(G_OBJECT(button5), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(functionRGB),button6);
    g_signal_connect(button6, "clicked", G_CALLBACK(on_button6_clicked), NULL);
    // g_object_set_data(G_OBJECT(button6), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(functionRGB),button7);
    g_signal_connect(button7, "clicked", G_CALLBACK(on_button7_clicked), NULL);
    // g_object_set_data(G_OBJECT(button7), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(functionRGB),button8);
    g_signal_connect(button8, "clicked", G_CALLBACK(on_button8_clicked), NULL);
    // g_object_set_data(G_OBJECT(button8), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(functionRGB),button9);
    g_signal_connect(button9, "clicked", G_CALLBACK(on_button9_clicked), NULL);
    // g_object_set_data(G_OBJECT(button9), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(functionRGB),button10);
    g_signal_connect(button10, "clicked", G_CALLBACK(on_button10_clicked), NULL);
    // g_object_set_data(G_OBJECT(button10), "image_widget", image);

    GtkWidget *historico = g_object_new(
        GTK_TYPE_BOX,
        "visible", TRUE,
        "orientation", GTK_ORIENTATION_VERTICAL,
        "spacing", 10,
        "homogeneous", FALSE,
        "valign", GTK_ALIGN_CENTER,
        "halign", GTK_ALIGN_CENTER,
        NULL
    );
    gtk_box_pack_start(GTK_BOX(right_box), historico, FALSE, FALSE, 0);

    GtkWidget *functionHistorico = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(historico), functionHistorico, FALSE, TRUE, 10);
    
    gtk_container_add(GTK_CONTAINER(functionHistorico),button11);
    g_signal_connect(button11, "clicked", G_CALLBACK(on_button11_clicked), NULL);
    // g_object_set_data(G_OBJECT(button11), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(functionHistorico),button12);
    g_signal_connect(button12, "clicked", G_CALLBACK(on_button12_clicked), NULL);
    // g_object_set_data(G_OBJECT(button12), "image_widget", image);

    gtk_container_add(GTK_CONTAINER(functionHistorico),button13);
    g_signal_connect(button13, "clicked", G_CALLBACK(on_button13_clicked), NULL);
    // g_object_set_data(G_OBJECT(button13), "image_widget", image);

    gtk_widget_show_all(window);
}