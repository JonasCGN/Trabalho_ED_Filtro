#include <gtk/gtk.h>
#include <stdlib.h>

#include "../functions/image.h"
#include "tela.h"

typedef struct {
    GtkWidget *entry1;
    GtkWidget *entry2;
    Appdata *app_data;
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
    HistoricoGray* aux = app_data->historicogray;

    do{
        aux = aux->prox;
    }while(aux->prox != app_data->historicogray);

    ImageGray *flipped_image = flip_vertical_gray(aux->imageGray);
    
    addFinalDuplamenteCircularGray(app_data->historicogray, flipped_image);

    GdkPixbuf *pixbuf = image_gray_to_pixbuf(flipped_image);
    if (app_data->image_widget_gray) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf);
    }

    g_object_unref(pixbuf);
}

void on_button2_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata*)user_data;
    HistoricoGray* aux = app_data->historicogray;

    do{
        aux = aux->prox;
    }while(aux->prox != app_data->historicogray);

    ImageGray *flipped_image = flip_horizontal_gray(aux->imageGray);

    addFinalDuplamenteCircularGray(app_data->historicogray, flipped_image);

    GdkPixbuf *pixbuf = image_gray_to_pixbuf(flipped_image);
    if (app_data->image_widget_gray) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf);
    }

    g_object_unref(pixbuf);
}

void on_button3_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata*)user_data;
    HistoricoGray* aux = app_data->historicogray;

    do{
        aux = aux->prox;
    }while(aux->prox != app_data->historicogray);

    ImageGray *flipped_image = transposeGray(aux->imageGray);

    addFinalDuplamenteCircularGray(app_data->historicogray, flipped_image);

    GdkPixbuf *pixbuf = image_gray_to_pixbuf(flipped_image);
    if (app_data->image_widget_gray) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf);
    }

    g_object_unref(pixbuf);
}

void on_dialog4_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
    if (response_id == GTK_RESPONSE_OK) {
        DialogData *dialog = (DialogData*)user_data;

        GtkEntry *entry = GTK_ENTRY(dialog->entry1);
        
        const gchar *text = gtk_entry_get_text(entry);
        int num = atoi(text);

        Appdata *app_data = (Appdata*)dialog->app_data;

        HistoricoGray *aux = app_data->historicogray;

        do{
            aux = aux->prox;
        }while(aux->prox != app_data->historicogray);

        ImageGray *flipped_image = median_blur_gray(aux->imageGray,num);

        addFinalDuplamenteCircularGray(app_data->historicogray, flipped_image);

        GdkPixbuf *pixbuf = image_gray_to_pixbuf(flipped_image);
        if (app_data->image_widget_gray) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf);
        }

        g_object_unref(pixbuf);
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void on_button4_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *entry;

    DialogData *dialog_data = (DialogData*)malloc(sizeof(DialogData));
    dialog_data->app_data = (Appdata*)user_data;
    
    GtkWindow *parent_window = GTK_WINDOW(NULL);

    const char *name = "Insira um número para a caixa";
    dialog = gtk_dialog_new_with_buttons(
        name,
        parent_window,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK",
        GTK_RESPONSE_OK,
        "_Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), strlen(name) * 12, -1);
    
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget *label = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do Kernel Size",
        NULL
    );

    gtk_container_add(GTK_CONTAINER(content_area), label);
    dialog_data->entry1 = gtk_entry_new();

    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry1);

    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog4_response), dialog_data);

    gtk_widget_show_all(dialog);
}

void on_dialog5_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
    if (response_id == GTK_RESPONSE_OK) {
        DialogData *dialog = (DialogData*)user_data;
        
        GtkEntry *entry1 = GTK_ENTRY(dialog->entry1);
        GtkEntry *entry2 = GTK_ENTRY(dialog->entry2);
        
        const gchar *text1 = gtk_entry_get_text(entry1);
        const gchar *text2 = gtk_entry_get_text(entry2);
        
        int num1 = atoi(text1);
        int num2 = atoi(text2);

        Appdata *app_data = (Appdata*)dialog->app_data;

        HistoricoGray *aux = app_data->historicogray;

        do{
            aux = aux->prox;
        }while(aux->prox != app_data->historicogray);

        ImageGray *flipped_image = clahe_gray(aux->imageGray,num1,num2);

        addFinalDuplamenteCircularGray(app_data->historicogray, flipped_image);

        GdkPixbuf *pixbuf = image_gray_to_pixbuf(flipped_image);
        if (app_data->image_widget_gray) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf);
        }

        g_object_unref(pixbuf);

    }
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void on_button5_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWindow *parent_window = GTK_WINDOW(NULL);

    DialogData *dialog_data = (DialogData*)g_malloc(sizeof(DialogData));
    dialog_data->app_data = (Appdata*)user_data;
    
    const char *name = "Insiras os número da caixa";
    // Criar um novo diálogo
    dialog = gtk_dialog_new_with_buttons(
        name,
        parent_window,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK",
        GTK_RESPONSE_OK,
        "_Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), strlen(name) * 12, -1);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget *label1 = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do tile width",
        NULL
    );

    gtk_container_add(GTK_CONTAINER(content_area), label1);
    dialog_data->entry1 = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry1);

    GtkWidget *label2 = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do tile heigth",
        NULL
    );

    gtk_container_add(GTK_CONTAINER(content_area), label2);
    dialog_data->entry2 = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry2);

    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog5_response), dialog_data);

    gtk_widget_show_all(dialog);
}

void on_button6_clicked(GtkButton *button, gpointer user_data) {
    Appdata *app_data = (Appdata*)user_data;
    HistoricoRGB* aux = app_data->historicorgb;

    do{
        aux = aux->prox;
    }while(aux->prox != app_data->historicorgb);

    // Flip the image horizontally
    ImageRGB *flipped_image = flip_vertical_rgb(aux->imageRGB);

    // Add the flipped image to the history
    addFinalDuplamenteCircularRGB(app_data->historicorgb, flipped_image);

    // Update the image widget with the new flipped image
    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(flipped_image);
    if (app_data->image_widget_rgb) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf);
    }

    // Free the GdkPixbuf when no longer needed
    g_object_unref(pixbuf);
}

void on_button7_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata*)user_data;
    HistoricoRGB* aux = app_data->historicorgb;

    do{
        aux = aux->prox;
    }while(aux->prox != app_data->historicorgb);

    ImageRGB *flipped_image = flip_horizontal_rgb(aux->imageRGB);

    addFinalDuplamenteCircularRGB(app_data->historicorgb, flipped_image);

    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(flipped_image);
    if (app_data->image_widget_rgb) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf);
    }

    g_object_unref(pixbuf);
}

void on_button8_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata*)user_data;
    HistoricoRGB* aux = app_data->historicorgb;

    do{
        aux = aux->prox;
    }while(aux->prox != app_data->historicorgb);

    // Flip the image horizontally
    ImageRGB *flipped_image = transposeRGB(aux->imageRGB);

    // Add the flipped image to the history
    addFinalDuplamenteCircularRGB(app_data->historicorgb, flipped_image);

    // Update the image widget with the new flipped image
    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(flipped_image);
    if (app_data->image_widget_rgb) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf);
    }

    // Free the GdkPixbuf when no longer needed
    g_object_unref(pixbuf);
}

void on_dialog9_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
    if (response_id == GTK_RESPONSE_OK) {
        DialogData *dialog = (DialogData*)user_data;

        GtkEntry *entry = GTK_ENTRY(dialog->entry1);
        
        const gchar *text = gtk_entry_get_text(entry);
        int num = atoi(text);

        Appdata *app_data = (Appdata*)dialog->app_data;

        HistoricoRGB *aux = app_data->historicorgb;

        do{
            aux = aux->prox;
        }while(aux->prox != app_data->historicorgb);

        ImageRGB *flipped_image = median_blur_rgb(aux->imageRGB,num);

        addFinalDuplamenteCircularRGB(app_data->historicorgb, flipped_image);

        GdkPixbuf *pixbuf = image_rgb_to_pixbuf(flipped_image);
        if (app_data->image_widget_rgb) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf);
        }

        g_object_unref(pixbuf);
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void on_button9_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *entry;

    DialogData *dialog_data = (DialogData*)malloc(sizeof(DialogData));
    dialog_data->app_data = (Appdata*)user_data;
    
    GtkWindow *parent_window = GTK_WINDOW(NULL);

    const char *name = "Insira um número para a caixa";
    dialog = gtk_dialog_new_with_buttons(
        name,
        parent_window,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK",
        GTK_RESPONSE_OK,
        "_Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), strlen(name) * 12, -1);
    
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget *label = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do Kernel Size",
        NULL
    );

    gtk_container_add(GTK_CONTAINER(content_area), label);
    dialog_data->entry1 = gtk_entry_new();

    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry1);

    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog9_response), dialog_data);

    gtk_widget_show_all(dialog);
}

void on_dialog10_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
    if (response_id == GTK_RESPONSE_OK) {
        DialogData *dialog = (DialogData*)user_data;
        
        GtkEntry *entry1 = GTK_ENTRY(dialog->entry1);
        GtkEntry *entry2 = GTK_ENTRY(dialog->entry2);
        
        const gchar *text1 = gtk_entry_get_text(entry1);
        const gchar *text2 = gtk_entry_get_text(entry2);
        
        int num1 = atoi(text1);
        int num2 = atoi(text2);

        Appdata *app_data = (Appdata*)dialog->app_data;
        HistoricoRGB *aux = app_data->historicorgb;

        do{
            aux = aux->prox;
        }while(aux->prox != app_data->historicorgb);

        ImageRGB *flipped_image = clahe_rgb(aux->imageRGB,num1,num2);

        addFinalDuplamenteCircularRGB(app_data->historicorgb, flipped_image);

        GdkPixbuf *pixbuf = image_rgb_to_pixbuf(flipped_image);
        if (app_data->image_widget_rgb) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf);
        }

        g_object_unref(pixbuf);

    }
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void on_button10_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWindow *parent_window = GTK_WINDOW(NULL);

    DialogData *dialog_data = (DialogData*)g_malloc(sizeof(DialogData));
    dialog_data->app_data = (Appdata*)user_data;
    
    const char *name = "Insiras os número da caixa";
    dialog = gtk_dialog_new_with_buttons(
        name,
        parent_window,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK",
        GTK_RESPONSE_OK,
        "_Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), strlen(name) * 12, -1);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget *label1 = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do tile width",
        NULL
    );

    gtk_container_add(GTK_CONTAINER(content_area), label1);
    dialog_data->entry1 = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry1);

    GtkWidget *label2 = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do tile heigth",
        NULL
    );

    gtk_container_add(GTK_CONTAINER(content_area), label2);
    dialog_data->entry2 = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry2);

    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog10_response), dialog_data);

    gtk_widget_show_all(dialog);
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

void on_button14_clicked(GtkButton *button, gpointer user_data)
{

    Appdata *app_data = (Appdata *)user_data;

    if (app_data == NULL || app_data->historicogray == NULL) {
        return; // Verifica se os ponteiros são válidos
    }

    HistoricoGray *aux = app_data->historicogray;

    if (aux->prox == NULL) {
        return; // Verifica se a lista tem pelo menos um elemento
    }

    // Navega para o último elemento da lista circular
    do {
        aux = aux->prox;
    } while (aux->prox != app_data->historicogray && aux->prox != NULL);

    // Gera uma nova imagem processada aleatoriamente
    ImageGray *random_image = random_gray(aux->imageGray);

    if (random_image != NULL) {
        // Atualiza a referência da imagem no app_data
        app_data->imagegray = random_image;

        // Adiciona a nova imagem ao histórico
        app_data->historicogray = addFinalDuplamenteCircularGray(app_data->historicogray, random_image);

        // Converte a imagem para GdkPixbuf e atualiza o widget GTK
        GdkPixbuf *pixbuf = image_gray_to_pixbuf(random_image);
        if (app_data->image_widget_gray) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf);
        }
        g_object_unref(pixbuf);
    }
}

GtkWidget *pagina1(gpointer user_data){
    GtkWidget *image;
    Appdata *app_data = (Appdata*)user_data;

    //Cria a Box da Janela
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    //Cria a Box da Imagem1
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(box), left_box, TRUE, TRUE, 10);
    
    // ImageGray *img = (ImageGray *)app_data->historicogray->imageGray;
    ImageGray *img = (ImageGray *)app_data->historicogray->imageGray;

    // GdkPixbuf *pixbuf = image_gray_to_pixbuf(img);
    GdkPixbuf *pixbuf = image_gray_to_pixbuf(img);

    image = gtk_image_new_from_pixbuf(pixbuf);
    gtk_box_pack_start(GTK_BOX(left_box), image, TRUE, TRUE, 0);

    app_data->image_widget_gray = image;

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

    GtkWidget *functionGray = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_box_pack_start(GTK_BOX(functions), functionGray, TRUE, TRUE, 0);

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
    GtkWidget *button14 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Random",
        NULL
    );

    gtk_container_add(GTK_CONTAINER(functionGray),button1);
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionGray),button2);
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button2_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionGray),button3);
    g_signal_connect(button3, "clicked", G_CALLBACK(on_button3_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionGray),button4);
    g_signal_connect(button4, "clicked", G_CALLBACK(on_button4_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionGray),button5);
    g_signal_connect(button5, "clicked", G_CALLBACK(on_button5_clicked), app_data);

      gtk_container_add(GTK_CONTAINER(functionGray),button5);
    g_signal_connect(button14, "clicked", G_CALLBACK(on_button14_clicked), app_data);

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
    gtk_box_pack_start(GTK_BOX(historico), functionHistorico, FALSE, FALSE, 10);
    
    gtk_container_add(GTK_CONTAINER(functionHistorico),button11);
    g_signal_connect(button11, "clicked", G_CALLBACK(on_button11_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionHistorico),button12);
    g_signal_connect(button12, "clicked", G_CALLBACK(on_button12_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionHistorico),button13);
    g_signal_connect(button13, "clicked", G_CALLBACK(on_button13_clicked), app_data);

    g_object_unref(pixbuf);

    return box;
}

GtkWidget *pagina2(gpointer user_data){
    GtkWidget *image;
    Appdata *app_data = (Appdata*)user_data;

    //Cria a Box da Janela
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    
    //Cria a Box da Imagem1
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(box), left_box, TRUE, TRUE, 10);
    
    // ImageGray *img = (ImageGray *)app_data->historicogray->imageGray;
    ImageRGB *img = (ImageRGB *)app_data->historicorgb->imageRGB;

    // GdkPixbuf *pixbuf = image_gray_to_pixbuf(img);
    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(img);

    image = gtk_image_new_from_pixbuf(pixbuf);
    gtk_box_pack_start(GTK_BOX(left_box), image, TRUE, TRUE, 0);

    app_data->image_widget_rgb = image;

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

    GtkWidget *functionRGB = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_box_pack_start(GTK_BOX(functions), functionRGB, TRUE, TRUE, 0);

    GtkWidget *button1 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Flip Vertical RGB",
        NULL
    ); 

    GtkWidget *button2 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Flip horizontal RGB",
        NULL
    ); 

    GtkWidget *button3 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Transpose RGB",
        NULL
    ); 

    GtkWidget *button4 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Media Blur RGB",
        NULL
    ); 

    GtkWidget *button5 = g_object_new(
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

    gtk_container_add(GTK_CONTAINER(functionRGB),button1);
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button6_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionRGB),button2);
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button7_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionRGB),button3);
    g_signal_connect(button3, "clicked", G_CALLBACK(on_button8_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionRGB),button4);
    g_signal_connect(button4, "clicked", G_CALLBACK(on_button9_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionRGB),button5);
    g_signal_connect(button5, "clicked", G_CALLBACK(on_button10_clicked), app_data);

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
    gtk_box_pack_start(GTK_BOX(historico), functionHistorico, FALSE, FALSE, 10);
    
    gtk_container_add(GTK_CONTAINER(functionHistorico),button11);
    g_signal_connect(button11, "clicked", G_CALLBACK(on_button11_clicked), NULL);

    gtk_container_add(GTK_CONTAINER(functionHistorico),button12);
    g_signal_connect(button12, "clicked", G_CALLBACK(on_button12_clicked), NULL);

    gtk_container_add(GTK_CONTAINER(functionHistorico),button13);
    g_signal_connect(button13, "clicked", G_CALLBACK(on_button13_clicked), NULL);

    g_object_unref(pixbuf);
    return box;
}

GtkWidget *pagina3(){
    
}

GtkWidget* criaPaginas(gpointer user_data) {
    GtkWidget *stack = g_object_new(
        GTK_TYPE_STACK,
        "visible", TRUE,
        "transition-type", GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT,
        NULL
    );

    GtkWidget *page1 = pagina1(user_data);
    GtkWidget *page2 = pagina2(user_data);
    // GtkWidget *page3 = crea();

    gtk_stack_add_titled(GTK_STACK(stack), page1, "page1", "Image Gray");
    gtk_stack_add_titled(GTK_STACK(stack), page2, "page2", "Image RGB");
    // gtk_stack_add_titled(GTK_STACK(stack), page3, "page3", "Page 3");

    return stack;
}

void app_activate(GApplication *app, gpointer user_data){
    GtkWidget *window;
    GtkWidget *switcher;
    GtkWidget *pages;

    window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "SWJ");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    //Cria a Box da Janela
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    pages = criaPaginas(user_data);

    switcher = g_object_new(
        GTK_TYPE_STACK_SWITCHER,
        "visible", TRUE,
        "stack", pages,
        "halign", GTK_ALIGN_END,
        NULL
    );

    gtk_box_pack_start(GTK_BOX(box), pages, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), switcher, FALSE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_widget_show_all(window);

}