#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
#include <Python.h>
#include <stdio.h>

#include "../image/image.h"
#include "../lista/list.h"
#include "tela.h"

typedef struct{
    GtkWidget *entry1;
    GtkWidget *entry2;
    Appdata *app_data;
} DialogData;

typedef struct{
    GtkWidget *entry;
    GtkWidget *radio_button1;
    GtkWidget *radio_button2;
    int opc;
    GtkWidget *file_chooser;
    GtkWidget *window;
    Appdata *app_data;
} SubmitData;

void pngToTxT(const char *name, int oP){
    PyObject *pModule, *pFunc, *pArgs, *pValue;

    Py_Initialize();

    PyObject *sys = PyImport_ImportModule("sys");
    PyObject *path = PyObject_GetAttrString(sys, "path");
    PyList_Append(path, PyUnicode_FromString("utils"));

    pModule = PyImport_ImportModule("image_utils");

    if (pModule != NULL){
        pFunc = PyObject_GetAttrString(pModule, "txt_from_image_gray");

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(3);
            PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(name)); // Primeiro argumento
            PyTuple_SetItem(pArgs, 1, PyUnicode_FromString("./utils/imagem_upload.txt")); // Segundo argumento
            PyTuple_SetItem(pArgs, 2, PyLong_FromLong(oP));

            pValue = PyObject_CallObject(pFunc, pArgs);

            if (pValue == NULL){
                PyErr_Print();
            }

            Py_DECREF(pArgs);
        }else{
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Não foi possível encontrar a função image_rgb_from_txt\n");
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }else{
        PyErr_Print();
        fprintf(stderr, "Falha ao importar o módulo functions\n");
    }
}

GdkPixbuf* image_rgb_to_pixbuf(ImageRGB *img){
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, img->dim.largura, img->dim.altura);

    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    int n_channels = gdk_pixbuf_get_n_channels(pixbuf);

    for (int i = 0; i < img->dim.altura; i++){
        for (int j = 0; j < img->dim.largura; j++){
            PixelRGB *src_pixel = &img->pixels[i * img->dim.largura + j];
            guchar *dest_pixel = pixels + i * rowstride + j * n_channels;
            dest_pixel[0] = src_pixel->red;
            dest_pixel[1] = src_pixel->green;
            dest_pixel[2] = src_pixel->blue;
        }
    }

    return pixbuf;
}

GdkPixbuf *image_gray_to_pixbuf(ImageGray *img){
    // Create a new GdkPixbuf
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, img->dim.largura, img->dim.altura);

    // Get the pixel array and row stride from the GdkPixbuf
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    int n_channels = gdk_pixbuf_get_n_channels(pixbuf);

    // Copy the pixels from ImageRGB to GdkPixbuf
    for (int i = 0; i < img->dim.altura; i++){
        for (int j = 0; j < img->dim.largura; j++){
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
    Appdata *app_data = (Appdata *)user_data;
    HistoricoGray *aux = app_data->historicogray;

    do{
        aux = aux->prox;
    } while (aux->prox != app_data->historicogray);

    ImageGray *flipped_image = flip_vertical_gray(aux->imageGray);
    app_data->imagegray = flipped_image;

    app_data->historicogray = addFinalDuplamenteCircularGray(app_data->historicogray, flipped_image);

    GdkPixbuf *pixbuf = image_gray_to_pixbuf(flipped_image);
    if (app_data->image_widget_gray){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf);
    }

    g_object_unref(pixbuf);
}

void on_button2_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data;
    HistoricoGray *aux = app_data->historicogray;

    do{
        aux = aux->prox;
    } while (aux->prox != app_data->historicogray);

    ImageGray *flipped_image = flip_horizontal_gray(aux->imageGray);
    app_data->imagegray = flipped_image;

    app_data->historicogray = addFinalDuplamenteCircularGray(app_data->historicogray, flipped_image);

    GdkPixbuf *pixbuf = image_gray_to_pixbuf(flipped_image);
    if (app_data->image_widget_gray){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf);
    }

    g_object_unref(pixbuf);
}

void on_button3_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data;
    HistoricoGray *aux = app_data->historicogray;

    do{
        aux = aux->prox;
    } while (aux->prox != app_data->historicogray);

    ImageGray *flipped_image = transposeGray(aux->imageGray);
    app_data->imagegray = flipped_image;

    app_data->historicogray = addFinalDuplamenteCircularGray(app_data->historicogray, flipped_image);

    GdkPixbuf *pixbuf = image_gray_to_pixbuf(flipped_image);
    if (app_data->image_widget_gray){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf);
    }

    g_object_unref(pixbuf);
}

void on_dialog4_response(GtkDialog *dialog, gint response_id, gpointer user_data){
    if (response_id == GTK_RESPONSE_OK){
        DialogData *dialog = (DialogData *)user_data;

        GtkEntry *entry = GTK_ENTRY(dialog->entry1);

        const gchar *text = gtk_entry_get_text(entry);
        int num = atoi(text);

        Appdata *app_data = (Appdata *)dialog->app_data;

        HistoricoGray *aux = app_data->historicogray;

        do{
            aux = aux->prox;
        } while (aux->prox != app_data->historicogray);

        ImageGray *flipped_image = median_blur_gray(aux->imageGray, num);
        app_data->imagegray = flipped_image;

        app_data->historicogray = addFinalDuplamenteCircularGray(app_data->historicogray, flipped_image);

        GdkPixbuf *pixbuf = image_gray_to_pixbuf(flipped_image);
        if (app_data->image_widget_gray){
            gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf);
        }

        g_object_unref(pixbuf);
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void on_button4_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *dialog;
    GtkWidget *content_area;

    DialogData *dialog_data = (DialogData *)malloc(sizeof(DialogData));
    dialog_data->app_data = (Appdata *)user_data;

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
        NULL);

    gtk_window_set_default_size(GTK_WINDOW(dialog), strlen(name) * 12, -1);

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget *label = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do Kernel Size",
        NULL);

    gtk_container_add(GTK_CONTAINER(content_area), label);
    dialog_data->entry1 = gtk_entry_new();

    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry1);

    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog4_response), dialog_data);

    gtk_widget_show_all(dialog);
}

void on_dialog5_response(GtkDialog *dialog, gint response_id, gpointer user_data){
    if (response_id == GTK_RESPONSE_OK){
        DialogData *dialog = (DialogData *)user_data;

        GtkEntry *entry1 = GTK_ENTRY(dialog->entry1);
        GtkEntry *entry2 = GTK_ENTRY(dialog->entry2);

        const gchar *text1 = gtk_entry_get_text(entry1);
        const gchar *text2 = gtk_entry_get_text(entry2);

        int num1 = atoi(text1);
        int num2 = atoi(text2);

        Appdata *app_data = (Appdata *)dialog->app_data;

        HistoricoGray *aux = app_data->historicogray;

        do{
            aux = aux->prox;
        } while (aux->prox != app_data->historicogray);

        ImageGray *flipped_image = clahe_gray(aux->imageGray, num1, num2);
        app_data->imagegray = flipped_image;

        app_data->historicogray = addFinalDuplamenteCircularGray(app_data->historicogray, flipped_image);

        GdkPixbuf *pixbuf = image_gray_to_pixbuf(flipped_image);
        if (app_data->image_widget_gray){
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

    DialogData *dialog_data = (DialogData *)g_malloc(sizeof(DialogData));
    dialog_data->app_data = (Appdata *)user_data;

    const char *name = "Insiras os número da caixa";

    dialog = gtk_dialog_new_with_buttons(
        name,
        parent_window,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK",
        GTK_RESPONSE_OK,
        "_Cancel",
        GTK_RESPONSE_CANCEL,
        NULL);

    gtk_window_set_default_size(GTK_WINDOW(dialog), strlen(name) * 12, -1);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget *label1 = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do tile width",
        NULL);

    gtk_container_add(GTK_CONTAINER(content_area), label1);
    dialog_data->entry1 = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry1);

    GtkWidget *label2 = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do tile heigth",
        NULL);

    gtk_container_add(GTK_CONTAINER(content_area), label2);
    dialog_data->entry2 = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry2);

    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog5_response), dialog_data);

    gtk_widget_show_all(dialog);
}

void on_button6_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data;
    HistoricoRGB *aux = app_data->historicorgb;

    do{
        aux = aux->prox;
    } while (aux->prox != app_data->historicorgb);

    ImageRGB *flipped_image = flip_vertical_rgb(aux->imageRGB);
    app_data->imagergb = flipped_image;

    app_data->historicorgb = addFinalDuplamenteCircularRGB(app_data->historicorgb, flipped_image);

    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(flipped_image);
    if (app_data->image_widget_rgb){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf);
    }

    g_object_unref(pixbuf);
}

void on_button7_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data;
    HistoricoRGB *aux = app_data->historicorgb;

    do{
        aux = aux->prox;
    } while (aux->prox != app_data->historicorgb);

    ImageRGB *flipped_image = flip_horizontal_rgb(aux->imageRGB);
    app_data->imagergb = flipped_image;

    app_data->historicorgb = addFinalDuplamenteCircularRGB(app_data->historicorgb, flipped_image);

    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(flipped_image);
    if (app_data->image_widget_rgb){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf);
    }

    g_object_unref(pixbuf);
}

void on_button8_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data;
    HistoricoRGB *aux = app_data->historicorgb;

    do{
        aux = aux->prox;
    } while (aux->prox != app_data->historicorgb);

    ImageRGB *flipped_image = transposeRGB(aux->imageRGB);
    app_data->imagergb = flipped_image;

    app_data->historicorgb = addFinalDuplamenteCircularRGB(app_data->historicorgb, flipped_image);

    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(flipped_image);
    if (app_data->image_widget_rgb){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf);
    }

    g_object_unref(pixbuf);
}

void on_dialog9_response(GtkDialog *dialog, gint response_id, gpointer user_data){
    if (response_id == GTK_RESPONSE_OK){
        DialogData *dialog = (DialogData *)user_data;

        GtkEntry *entry = GTK_ENTRY(dialog->entry1);

        const gchar *text = gtk_entry_get_text(entry);
        int num = atoi(text);

        Appdata *app_data = (Appdata *)dialog->app_data;

        HistoricoRGB *aux = app_data->historicorgb;

        do{
            aux = aux->prox;
        } while (aux->prox != app_data->historicorgb);

        ImageRGB *flipped_image = median_blur_rgb(aux->imageRGB, num);
        app_data->imagergb = flipped_image;

        app_data->historicorgb = addFinalDuplamenteCircularRGB(app_data->historicorgb, flipped_image);

        GdkPixbuf *pixbuf = image_rgb_to_pixbuf(flipped_image);
        if (app_data->image_widget_rgb){
            gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf);
        }

        g_object_unref(pixbuf);
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void on_button9_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *dialog;
    GtkWidget *content_area;

    DialogData *dialog_data = (DialogData *)malloc(sizeof(DialogData));
    dialog_data->app_data = (Appdata *)user_data;

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
        NULL);

    gtk_window_set_default_size(GTK_WINDOW(dialog), strlen(name) * 12, -1);

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget *label = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do Kernel Size",
        NULL);

    gtk_container_add(GTK_CONTAINER(content_area), label);
    dialog_data->entry1 = gtk_entry_new();

    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry1);

    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog9_response), dialog_data);

    gtk_widget_show_all(dialog);
}

void on_dialog10_response(GtkDialog *dialog, gint response_id, gpointer user_data){
    if (response_id == GTK_RESPONSE_OK){
        DialogData *dialog = (DialogData *)user_data;

        GtkEntry *entry1 = GTK_ENTRY(dialog->entry1);
        GtkEntry *entry2 = GTK_ENTRY(dialog->entry2);

        const gchar *text1 = gtk_entry_get_text(entry1);
        const gchar *text2 = gtk_entry_get_text(entry2);

        int num1 = atoi(text1);
        int num2 = atoi(text2);

        Appdata *app_data = (Appdata *)dialog->app_data;
        HistoricoRGB *aux = app_data->historicorgb;

        do{
            aux = aux->prox;
        } while (aux->prox != app_data->historicorgb);

        ImageRGB *flipped_image = clahe_rgb(aux->imageRGB, num1, num2);
        app_data->imagergb = flipped_image;

        app_data->historicorgb = addFinalDuplamenteCircularRGB(app_data->historicorgb, flipped_image);

        GdkPixbuf *pixbuf = image_rgb_to_pixbuf(flipped_image);
        if (app_data->image_widget_rgb){
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

    DialogData *dialog_data = (DialogData *)g_malloc(sizeof(DialogData));
    dialog_data->app_data = (Appdata *)user_data;

    const char *name = "Insiras os número da caixa";
    dialog = gtk_dialog_new_with_buttons(
        name,
        parent_window,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK",
        GTK_RESPONSE_OK,
        "_Cancel",
        GTK_RESPONSE_CANCEL,
        NULL);

    gtk_window_set_default_size(GTK_WINDOW(dialog), strlen(name) * 12, -1);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget *label1 = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do tile width",
        NULL);

    gtk_container_add(GTK_CONTAINER(content_area), label1);
    dialog_data->entry1 = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry1);

    GtkWidget *label2 = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do tile heigth",
        NULL);

    gtk_container_add(GTK_CONTAINER(content_area), label2);
    dialog_data->entry2 = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry2);

    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog10_response), dialog_data);

    gtk_widget_show_all(dialog);
}

void on_button11_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data;

    HistoricoGray *aux = app_data->historicogray;

    while (aux->imageGray != app_data->imagegray){
        aux = aux->prox;
    }
    app_data->imagegray = aux->ant->imageGray;

    GdkPixbuf *pixbuf = image_gray_to_pixbuf(aux->ant->imageGray);
    if (app_data->image_widget_gray){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf);
    }
}

void on_button12_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data;

    HistoricoGray *historico = app_data->historicogray;
    HistoricoGray *aux = app_data->historicogray;

    do{
        aux = aux->prox;
    } while (aux->prox->imageGray != app_data->imagegray);

    app_data->historicogray = removerElementoGray(historico, app_data->imagegray);

    // Possivel erro
    app_data->imagegray = aux->prox->imageGray;

    GdkPixbuf *pixbuf = image_gray_to_pixbuf(aux->prox->imageGray);
    if (app_data->image_widget_gray){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf);
    }
}

void on_button13_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data;
    HistoricoGray *aux = app_data->historicogray;

    while (aux->imageGray != app_data->imagegray){
        aux = aux->prox;
    }
    app_data->imagegray = aux->prox->imageGray;

    GdkPixbuf *pixbuf = image_gray_to_pixbuf(aux->prox->imageGray);
    if (app_data->image_widget_gray){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf);
    }
}

void on_button14_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data;
    HistoricoRGB *aux = app_data->historicorgb;

    while (aux->imageRGB != app_data->imagergb){
        aux = aux->prox;
    }
    app_data->imagergb = aux->ant->imageRGB;

    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(aux->ant->imageRGB);
    if (app_data->image_widget_rgb){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf);
    }
}

void on_button15_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data;

    HistoricoRGB *historico = app_data->historicorgb;
    HistoricoRGB *aux = app_data->historicorgb;

    do{
        aux = aux->prox;
    } while (aux->prox->imageRGB != app_data->imagergb);

    app_data->historicorgb = removerElementoRGB(historico, app_data->imagergb);
    
    app_data->imagergb = aux->prox->imageRGB;

    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(aux->prox->imageRGB);
    if (app_data->image_widget_rgb){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf);
    }
}

void on_button16_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data;
    HistoricoRGB *aux = app_data->historicorgb;

    while (aux->imageRGB != app_data->imagergb){
        aux = aux->prox;
    }
    app_data->imagergb = aux->prox->imageRGB;

    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(aux->prox->imageRGB);
    if(app_data->image_widget_rgb){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf);
    }
}

void on_button17_clicked(GtkButton *button, gpointer user_data) {
    SubmitData *submit_data = (SubmitData *)user_data;
    Appdata *app_data = submit_data->app_data;

    if (submit_data->opc == 1) {
        HistoricoRandomGray *aux = app_data->histoRandomGray;

        while (aux->imageGray != app_data->imagerandomGray && aux->prox != NULL) {
            aux = aux->prox;
        }

        if (aux->prox != NULL) {
            app_data->imagerandomGray = aux->prox->imageGray;

            GdkPixbuf *pixbuf = image_gray_to_pixbuf(aux->prox->imageGray);
            if (pixbuf && app_data->image_widget_random) {
                gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_random), pixbuf);
            }
        }else{
            liberaListaRandomGray(app_data->histoRandomGray);
            app_data->histoRandomGray = NULL;
            gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(submit_data->file_chooser));
            gtk_widget_set_sensitive(GTK_WIDGET(submit_data->file_chooser), TRUE);
            submit_data->opc = 0;
        }
    }else if(submit_data->opc == 2){
        HistoricoRandomRGB *aux = app_data->histoRandomRGB;

        while (aux->imageRGB != app_data->imagerandomRGB && aux->prox != NULL) {
            aux = aux->prox;
        }

        if (aux->prox != NULL){
            app_data->imagerandomRGB = aux->prox->imageRGB;

            GdkPixbuf *pixbuf = image_rgb_to_pixbuf(aux->prox->imageRGB);
            if (pixbuf && app_data->image_widget_random) {
                gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_random), pixbuf);
            }
        }else{
            liberaListaRandomRGB(app_data->histoRandomRGB); 
            app_data->histoRandomRGB = NULL;
            
            gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(submit_data->file_chooser));
            gtk_widget_set_sensitive(GTK_WIDGET(submit_data->file_chooser), TRUE);
            submit_data->opc = 0;
        }
    }
}

void on_submited_info(GtkWidget *widget, gpointer user_data) {
    SubmitData *submit_data = (SubmitData *)user_data;
    Appdata *app_data = submit_data->app_data;
    FILE *arq;
    HistoricoRandomGray *histoGray = NULL;
    HistoricoRandomRGB *histoRGB = NULL;

    const gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(submit_data->file_chooser));

    if (filename == NULL){
        g_print("Nenhum arquivo selecionado.\n");
        return;
    }

    const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(submit_data->entry));
    int num = atoi(entry_text);
    
    srand(time(NULL));
    if(num != 0){
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(submit_data->radio_button1))) {
            int op;
            submit_data->opc = 1;

            pngToTxT(filename, 1);
            
            arq = fopen("./utils/imagem_upload.txt", "r");
            if (!arq) {
                g_print("Erro ao abrir o arquivo.\n");
                return;
            }

            ImageGray *image = create_image_gray(arq);

            ImageGray *result = image;
            histoGray = lista_randon_Gray(histoGray, image);
            
            for (int i = 0; i < num; i++) {
                op = rand() % 5;
                result = random_gray(result,op);
                histoGray = lista_randon_Gray(histoGray, result);
            }

            app_data->imagerandomGray = histoGray->imageGray;
            
            GdkPixbuf *pixbuf = image_gray_to_pixbuf(histoGray->imageGray);
            if (pixbuf && app_data->image_widget_random) {
                gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_random), pixbuf);
            }

            if(histoGray != NULL)
                app_data->histoRandomGray = histoGray;

        } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(submit_data->radio_button2))) {
            submit_data->opc = 2;

            pngToTxT(filename, 0);
            
            arq = fopen("./utils/imagem_upload.txt", "r");
            if(!arq){
                g_print("Erro ao abrir o arquivo.\n");
                return;
            }

            ImageRGB *image = create_image_rgb(arq);

            histoRGB = random_RGB(image, num);
            app_data->imagerandomRGB = histoRGB->imageRGB;

            GdkPixbuf *pixbuf = image_rgb_to_pixbuf(histoRGB->imageRGB);
            if(pixbuf && app_data->image_widget_random){
                gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_random), pixbuf);
            }

            if(histoRGB != NULL)
                app_data->histoRandomRGB = histoRGB;

        }else{
            g_print("Nenhuma opção selecionada, valor de entrada: %d, arquivo: %s\n", num, filename);
        }

        gtk_widget_destroy(GTK_WIDGET(submit_data->window));
    }

}

void on_file_selected(GtkFileChooserButton *filechooserbutton, gpointer user_data) {
    SubmitData *submit_data = (SubmitData *)user_data;
    Appdata *app_data = submit_data->app_data;
    const gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooserbutton));

    if(filename != NULL){
        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
        if (pixbuf && app_data->image_widget_random){
            gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_random), pixbuf);
        }

        gtk_widget_set_sensitive(GTK_WIDGET(filechooserbutton), FALSE);

        GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "Insira Funções Random");
        gtk_window_set_default_size(GTK_WINDOW(window), 400, 170);
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
        submit_data->window = window;

        GtkWidget *content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        gtk_container_set_border_width(GTK_CONTAINER(content_area), 10);
        gtk_container_add(GTK_CONTAINER(window), content_area);

        GtkWidget *entry = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Digite a quantidade de operações randomicas que voce deseja fazer");
        gtk_box_pack_start(GTK_BOX(content_area), entry, FALSE, FALSE, 0);
        submit_data->entry = entry;

        GtkWidget *radio_button1 = gtk_radio_button_new_with_label(NULL, "Função Random Imagem Gray");
        gtk_box_pack_start(GTK_BOX(content_area), radio_button1, FALSE, FALSE, 0);
        submit_data->radio_button1 = radio_button1;

        GtkWidget *radio_button2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_button1), "Função Random Imagem RGB");
        gtk_box_pack_start(GTK_BOX(content_area), radio_button2, FALSE, FALSE, 0);
        submit_data->radio_button2 = radio_button2;
        
        GtkWidget *button = gtk_button_new_with_label("Iniciar");
        g_signal_connect(button, "clicked", G_CALLBACK(on_submited_info), submit_data);
        gtk_box_pack_start(GTK_BOX(content_area), button, FALSE, FALSE, 0);

        gtk_widget_show_all(window);
    }
}

GtkWidget *pagina1(gpointer user_data){
    GtkWidget *image;
    Appdata *app_data = (Appdata*)user_data;

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(box), left_box, TRUE, TRUE, 10);
    
    ImageGray *img = (ImageGray *)app_data->historicogray->imageGray;
    app_data->imagegray = img;

    GdkPixbuf *pixbuf = image_gray_to_pixbuf(img);

    image = gtk_image_new_from_pixbuf(pixbuf);
    gtk_box_pack_start(GTK_BOX(left_box), image, TRUE, TRUE, 0);

    app_data->image_widget_gray = image;

    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(box), right_box, FALSE, TRUE, 10);

    GtkWidget *titleTop = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Filtros Disponiveis",
        NULL);

    gtk_container_add(GTK_CONTAINER(right_box), titleTop);

    GtkWidget *functions = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(right_box), functions, FALSE, FALSE, 0);

    GtkWidget *functionGray = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_box_pack_start(GTK_BOX(functions), functionGray, TRUE, TRUE, 0);

    GtkWidget *button1 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Flip Vertical Gray",
        NULL);

    GtkWidget *button2 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Flip horizontal Gray",
        NULL);

    GtkWidget *button3 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Transpose Gray",
        NULL);

    GtkWidget *button4 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Media Blur Gray",
        NULL);

    GtkWidget *button5 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Clahe Gray",
        NULL);

    GtkWidget *button11 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "<<",
        NULL);

    GtkWidget *button12 = gtk_button_new();
    GtkWidget *lixeira = gtk_image_new_from_file("./image/lixeira.png");
    if(!gtk_image_get_pixbuf(GTK_IMAGE(lixeira))){
        g_print("Falha ao carregar a imagem\n");
    }else{
        gtk_button_set_image(GTK_BUTTON(button12), lixeira);
    }

    GtkWidget *button13 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", ">>",
        NULL);

    gtk_container_add(GTK_CONTAINER(functionGray), button1);
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionGray), button2);
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button2_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionGray), button3);
    g_signal_connect(button3, "clicked", G_CALLBACK(on_button3_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionGray), button4);
    g_signal_connect(button4, "clicked", G_CALLBACK(on_button4_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionGray), button5);
    g_signal_connect(button5, "clicked", G_CALLBACK(on_button5_clicked), app_data);

    GtkWidget *historico = g_object_new(
        GTK_TYPE_BOX,
        "visible", TRUE,
        "orientation", GTK_ORIENTATION_VERTICAL,
        "spacing", 10,
        "homogeneous", FALSE,
        "valign", GTK_ALIGN_CENTER,
        "halign", GTK_ALIGN_CENTER,
        NULL);
    gtk_box_pack_start(GTK_BOX(right_box), historico, FALSE, FALSE, 0);

    GtkWidget *functionHistorico = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(historico), functionHistorico, FALSE, FALSE, 10);

    gtk_container_add(GTK_CONTAINER(functionHistorico), button11);
    g_signal_connect(button11, "clicked", G_CALLBACK(on_button11_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionHistorico), button12);
    g_signal_connect(button12, "clicked", G_CALLBACK(on_button12_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionHistorico), button13);
    g_signal_connect(button13, "clicked", G_CALLBACK(on_button13_clicked), app_data);

    g_object_unref(pixbuf);

    return box;
}

GtkWidget *pagina2(gpointer user_data){
    GtkWidget *image;
    Appdata *app_data = (Appdata *)user_data;

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(box), left_box, TRUE, TRUE, 10);

    ImageRGB *img = (ImageRGB *)app_data->historicorgb->imageRGB;
    app_data->imagergb = img;

    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(img);

    image = gtk_image_new_from_pixbuf(pixbuf);
    gtk_box_pack_start(GTK_BOX(left_box), image, TRUE, TRUE, 0);

    app_data->image_widget_rgb = image;

    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(box), right_box, FALSE, TRUE, 10);

    GtkWidget *titleTop = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Filtros Disponiveis",
        NULL);

    gtk_container_add(GTK_CONTAINER(right_box), titleTop);

    GtkWidget *functions = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(right_box), functions, FALSE, FALSE, 0);

    GtkWidget *functionRGB = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_box_pack_start(GTK_BOX(functions), functionRGB, TRUE, TRUE, 0);

    GtkWidget *button1 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Flip Vertical RGB",
        NULL);

    GtkWidget *button2 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Flip horizontal RGB",
        NULL);

    GtkWidget *button3 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Transpose RGB",
        NULL);

    GtkWidget *button4 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Media Blur RGB",
        NULL);

    GtkWidget *button5 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Clahe RGB",
        NULL);

    GtkWidget *button11 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "<<",
        NULL);

    GtkWidget *button12 = gtk_button_new();
    GtkWidget *lixeira = gtk_image_new_from_file("./image/lixeira.png");
    if (!gtk_image_get_pixbuf(GTK_IMAGE(lixeira))){
        g_print("Falha ao carregar a imagem\n");
    }
    else{
        gtk_button_set_image(GTK_BUTTON(button12), lixeira);
    }

    GtkWidget *button13 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", ">>",
        NULL);

    gtk_container_add(GTK_CONTAINER(functionRGB), button1);
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button6_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionRGB), button2);
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button7_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionRGB), button3);
    g_signal_connect(button3, "clicked", G_CALLBACK(on_button8_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionRGB), button4);
    g_signal_connect(button4, "clicked", G_CALLBACK(on_button9_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionRGB), button5);
    g_signal_connect(button5, "clicked", G_CALLBACK(on_button10_clicked), app_data);

    GtkWidget *historico = g_object_new(
        GTK_TYPE_BOX,
        "visible", TRUE,
        "orientation", GTK_ORIENTATION_VERTICAL,
        "spacing", 10,
        "homogeneous", FALSE,
        "valign", GTK_ALIGN_CENTER,
        "halign", GTK_ALIGN_CENTER,
        NULL);
    gtk_box_pack_start(GTK_BOX(right_box), historico, FALSE, FALSE, 0);

    GtkWidget *functionHistorico = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(historico), functionHistorico, FALSE, FALSE, 10);

    gtk_container_add(GTK_CONTAINER(functionHistorico), button11);
    g_signal_connect(button11, "clicked", G_CALLBACK(on_button14_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionHistorico), button12);
    g_signal_connect(button12, "clicked", G_CALLBACK(on_button15_clicked), app_data);

    gtk_container_add(GTK_CONTAINER(functionHistorico), button13);
    g_signal_connect(button13, "clicked", G_CALLBACK(on_button16_clicked), app_data);

    g_object_unref(pixbuf);
    return box;
}

GtkWidget *pagina0(gpointer user_data){
    Appdata *app_data = (Appdata *)user_data;
    SubmitData *submit_data = g_malloc(sizeof(SubmitData));
    submit_data->app_data = app_data;

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(box), left_box, TRUE, TRUE, 10);

    GtkWidget *image = gtk_image_new();
    gtk_box_pack_start(GTK_BOX(left_box), image, TRUE, TRUE, 0);
    app_data->image_widget_random = image;

    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(box), right_box, FALSE, TRUE, 10);

    GtkWidget *spacer_top = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(right_box), spacer_top, TRUE, TRUE, 0);

    GtkWidget *file_chooser = g_object_new(
        GTK_TYPE_FILE_CHOOSER_BUTTON,
        "title", "Selecionar Imagem",
        "action", GTK_FILE_CHOOSER_ACTION_OPEN,
        "visible", TRUE,
        NULL
    );
    g_signal_connect(G_OBJECT(file_chooser), "file-set", G_CALLBACK(on_file_selected), submit_data);
    gtk_box_pack_start(GTK_BOX(right_box), file_chooser, FALSE, FALSE, 0);
    submit_data->file_chooser = file_chooser;

    GtkWidget *button1 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", ">>",
        NULL
    );
    gtk_container_add(GTK_CONTAINER(right_box), button1);
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button17_clicked), submit_data);

    GtkWidget *spacer_bottom = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(right_box), spacer_bottom, TRUE, TRUE, 0);

    gtk_widget_set_halign(file_chooser, GTK_ALIGN_CENTER);

    return box;
}

GtkWidget *criaPaginas(gpointer user_data){
    GtkWidget *stack = g_object_new(
        GTK_TYPE_STACK,
        "visible", TRUE,
        "transition-type", GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT,
        NULL);

    GtkWidget *page0 = pagina0(user_data);
    GtkWidget *page1 = pagina1(user_data);
    GtkWidget *page2 = pagina2(user_data);

    gtk_stack_add_titled(GTK_STACK(stack), page0, "page0", "Page Inicial");
    gtk_stack_add_titled(GTK_STACK(stack), page1, "page1", "Image Gray");
    gtk_stack_add_titled(GTK_STACK(stack), page2, "page2", "Image RGB");

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

    // Cria a Box da Janela
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    pages = criaPaginas(user_data);

    switcher = g_object_new(
        GTK_TYPE_STACK_SWITCHER,
        "visible", TRUE,
        "stack", pages,
        "halign", GTK_ALIGN_END,
        NULL);
    gtk_box_pack_start(GTK_BOX(box), pages, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), switcher, FALSE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_widget_show_all(window);
}
