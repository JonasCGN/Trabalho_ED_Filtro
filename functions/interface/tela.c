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
// Funções para manipulação de imagens
void pngToTxT(const char *name, int oP) {
    PyObject *pModule, *pFunc, *pArgs, *pValue;

    Py_Initialize(); // Inicializa o interpretador Python

    PyObject *sys = PyImport_ImportModule("sys"); // Importa o módulo sys para manipular o sys.path
    PyObject *path = PyObject_GetAttrString(sys, "path"); // Obtém o atributo 'path' do módulo sys
    PyList_Append(path, PyUnicode_FromString("utils")); // Adiciona o diretório "utils" ao sys.path

    pModule = PyImport_ImportModule("image_utils"); // Importa o módulo image_utils

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, "txt_from_image_gray"); // Obtém a função txt_from_image_gray

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(3); // Cria uma nova tupla Python para os argumentos da função
            PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(name)); // Define o primeiro argumento como o nome do arquivo PNG
            PyTuple_SetItem(pArgs, 1, PyUnicode_FromString("./utils/imagem_upload.txt")); // Define o segundo argumento como o caminho para o arquivo de saída
            PyTuple_SetItem(pArgs, 2, PyLong_FromLong(oP)); // Define o terceiro argumento como a opção (inteiro)

            pValue = PyObject_CallObject(pFunc, pArgs); // Chama a função com os argumentos fornecidos

            if (pValue == NULL) {
                PyErr_Print(); // Imprime o erro Python se a chamada falhar
            }

            Py_DECREF(pArgs); // Libera a tupla de argumentos
        } else {
            if (PyErr_Occurred())
                PyErr_Print(); // Imprime o erro se houver algum problema
            fprintf(stderr, "Não foi possível encontrar a função txt_from_image_gray\n"); // Imprime um erro se a função não foi encontrada
        }
        Py_XDECREF(pFunc); // Libera a referência à função
        Py_DECREF(pModule); // Libera a referência ao módulo
    } else {
        PyErr_Print(); // Imprime o erro se o módulo não puder ser importado
        fprintf(stderr, "Falha ao importar o módulo image_utils\n"); // Imprime uma mensagem de erro
    }

    Py_Finalize(); // Finaliza o interpretador Python
}


GdkPixbuf* image_rgb_to_pixbuf(ImageRGB *img) {
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, img->dim.largura, img->dim.altura); // Cria um novo GdkPixbuf com espaço de cor RGB, sem canal alfa, 8 bits por canal, e com as dimensões da imagem

    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf); // Obtém o ponteiro para o array de pixels do GdkPixbuf
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf); // Obtém o número de bytes por linha de pixels no GdkPixbuf
    int n_channels = gdk_pixbuf_get_n_channels(pixbuf); // Obtém o número de canais por pixel (deve ser 3 para RGB)

    for (int i = 0; i < img->dim.altura; i++) { // Itera sobre cada linha da imagem
        for (int j = 0; j < img->dim.largura; j++) { // Itera sobre cada coluna da imagem
            PixelRGB *src_pixel = &img->pixels[i * img->dim.largura + j]; // Obtém o pixel RGB da imagem de origem
            guchar *dest_pixel = pixels + i * rowstride + j * n_channels; // Calcula o ponteiro para o pixel correspondente no GdkPixbuf
            dest_pixel[0] = src_pixel->red; // Copia o valor vermelho do pixel de origem para o pixel de destino
            dest_pixel[1] = src_pixel->green; // Copia o valor verde do pixel de origem para o pixel de destino
            dest_pixel[2] = src_pixel->blue; // Copia o valor azul do pixel de origem para o pixel de destino
        }
    }

    return pixbuf; // Retorna o GdkPixbuf criado
}

GdkPixbuf *image_gray_to_pixbuf(ImageGray *img) {
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, img->dim.largura, img->dim.altura); // Cria um novo GdkPixbuf com espaço de cor RGB, sem canal alfa, 8 bits por canal, e com as dimensões da imagem

    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf); // Obtém o ponteiro para o array de pixels do GdkPixbuf
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf); // Obtém o número de bytes por linha de pixels no GdkPixbuf
    int n_channels = gdk_pixbuf_get_n_channels(pixbuf); // Obtém o número de canais por pixel (deve ser 3 para RGB)

    for (int i = 0; i < img->dim.altura; i++) { // Itera sobre cada linha da imagem
        for (int j = 0; j < img->dim.largura; j++) { // Itera sobre cada coluna da imagem
            PixelGray *src_pixel = &img->pixels[i * img->dim.largura + j]; // Obtém o pixel em escala de cinza da imagem de origem
            guchar *dest_pixel = pixels + i * rowstride + j * n_channels; // Calcula o ponteiro para o pixel correspondente no GdkPixbuf
            dest_pixel[0] = src_pixel->value; // Copia o valor em escala de cinza para o canal vermelho do pixel de destino
            dest_pixel[1] = src_pixel->value; // Copia o valor em escala de cinza para o canal verde do pixel de destino
            dest_pixel[2] = src_pixel->value; // Copia o valor em escala de cinza para o canal azul do pixel de destino
        }
    }

    return pixbuf; // Retorna o GdkPixbuf criado
}


void on_button1_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data; // Obtém a estrutura de dados da aplicação
    HistoricoGray *aux = app_data->historicogray; // Cria um ponteiro para percorrer a lista de histórico de imagens em escala de cinza
    // Percorre a lista até o último elemento
    do{
        aux = aux->prox;
    } while (aux->prox != app_data->historicogray);

    ImageGray *flipped_image = flip_vertical_gray(aux->imageGray); // Espelha a imagem verticalmente
    app_data->imagegray = flipped_image; // Atualiza a imagem atual para a imagem espelhada

    app_data->historicogray = addFinalDuplamenteCircularGray(app_data->historicogray, flipped_image); // Adiciona a imagem espelhada ao histórico

    GdkPixbuf *pixbuf = image_gray_to_pixbuf(flipped_image); // Converte a imagem espelhada para um GdkPixbuf
    if (app_data->image_widget_gray){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf);
    }
    // Atualiza a imagem exibida no widget da interface
    g_object_unref(pixbuf);
}

void on_button2_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data; // Obtém a estrutura de dados da aplicação
    HistoricoGray *aux = app_data->historicogray; // Cria um ponteiro para percorrer a lista de histórico de imagens em escala de cinza

    do{
        aux = aux->prox;
    } while (aux->prox != app_data->historicogray);
    // Percorre a lista até o último elemento
    ImageGray *flipped_image = flip_horizontal_gray(aux->imageGray); // Espelha a imagem horizontalmente
    app_data->imagegray = flipped_image; // Atualiza a imagem atual para a imagem espelhada

    app_data->historicogray = addFinalDuplamenteCircularGray(app_data->historicogray, flipped_image); // Adiciona a imagem espelhada ao histórico

    GdkPixbuf *pixbuf = image_gray_to_pixbuf(flipped_image); // Converte a imagem espelhada para um GdkPixbuf
    if (app_data->image_widget_gray){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf);
    }
    // Atualiza a imagem exibida no widget da interface
    g_object_unref(pixbuf);
}

void on_button3_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data; // Obtém a estrutura de dados da aplicação
    HistoricoGray *aux = app_data->historicogray; // Cria um ponteiro para percorrer a lista de histórico de imagens em escala de cinza

    do{
        aux = aux->prox;
    } while (aux->prox != app_data->historicogray);
    // Percorre a lista até o último elemento
    ImageGray *flipped_image = transposeGray(aux->imageGray); // Transpõe a imagem
    app_data->imagegray = flipped_image; // Atualiza a imagem atual para a imagem transposta

    app_data->historicogray = addFinalDuplamenteCircularGray(app_data->historicogray, flipped_image); // Adiciona a imagem transposta ao histórico

    GdkPixbuf *pixbuf = image_gray_to_pixbuf(flipped_image); // Converte a imagem transposta para um GdkPixbuf
    if (app_data->image_widget_gray){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf);
    }
    // Atualiza a imagem exibida no widget da interface
    g_object_unref(pixbuf);
}

void on_dialog4_response(GtkDialog *dialog, gint response_id, gpointer user_data){
    if (response_id == GTK_RESPONSE_OK){
        DialogData *dialog = (DialogData *)user_data; // Obtém os dados do diálogo

        GtkEntry *entry = GTK_ENTRY(dialog->entry1); // Obtém o campo de entrada do diálogo

        const gchar *text = gtk_entry_get_text(entry); // Obtém o texto digitado no campo de entrada
        int num = atoi(text); // Converte o texto para um número inteiro

        if(num != 0){
            Appdata *app_data = (Appdata *)dialog->app_data; // Obtém a estrutura de dados da aplicação

            HistoricoGray *aux = app_data->historicogray; // Cria um ponteiro para percorrer a lista de histórico de imagens em escala de cinza

            do{
                aux = aux->prox;
            } while (aux->prox != app_data->historicogray);
            // Percorre a lista até o último elemento
            ImageGray *flipped_image = median_blur_gray(aux->imageGray, num); // Aplica o filtro de mediana na imagem
            app_data->imagegray = flipped_image; // Atualiza a imagem atual para a imagem com o filtro aplicado

            app_data->historicogray = addFinalDuplamenteCircularGray(app_data->historicogray, flipped_image); // Adiciona a imagem com o filtro ao histórico

            GdkPixbuf *pixbuf = image_gray_to_pixbuf(flipped_image); // Converte a imagem com o filtro para um GdkPixbuf
            if (app_data->image_widget_gray){
                gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf);
            }
            // Atualiza a imagem exibida no widget da interface
            g_object_unref(pixbuf);
            
        }
        
    }
    gtk_widget_destroy(GTK_WIDGET(dialog)); // Fecha o diálogo
}

void on_button4_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *dialog; // Cria um ponteiro para o diálogo
    GtkWidget *content_area; // Cria um ponteiro para a área de conteúdo do diálogo

    DialogData *dialog_data = (DialogData *)malloc(sizeof(DialogData)); // Aloca memória para os dados do diálogo
    dialog_data->app_data = (Appdata *)user_data; // Define a estrutura de dados da aplicação nos dados do diálogo

    GtkWindow *parent_window = GTK_WINDOW(NULL); // Obtém a janela principal

    const char *name = "Insira um número para a caixa"; // Define o nome do diálogo
    dialog = gtk_dialog_new_with_buttons(
        name,
        parent_window,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK",
        GTK_RESPONSE_OK,
        "_Cancel",
        GTK_RESPONSE_CANCEL,
        NULL);

    gtk_window_set_default_size(GTK_WINDOW(dialog), strlen(name) * 12, -1); // Define o tamanho padrão do diálogo

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER); // Define a posição do diálogo

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog)); // Obtém a área de conteúdo do diálogo

    GtkWidget *label = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do Kernel Size",
        NULL); // Cria um novo rótulo com o texto "Digite o tamanho do Kernel Size"

    gtk_container_add(GTK_CONTAINER(content_area), label); // Adiciona o rótulo à área de conteúdo do diálogo
    dialog_data->entry1 = gtk_entry_new(); // Cria um novo campo de entrada

    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry1); // Adiciona o campo de entrada à área de conteúdo do diálogo

    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog4_response), dialog_data); // Conecta o sinal "response" do diálogo à função de callback

    gtk_widget_show_all(dialog); // Exibe o diálogo
}

void on_dialog5_response(GtkDialog *dialog, gint response_id, gpointer user_data){
    if (response_id == GTK_RESPONSE_OK){
        DialogData *dialog = (DialogData *)user_data; // Obtém os dados do diálogo

        GtkEntry *entry1 = GTK_ENTRY(dialog->entry1); // Obtém o campo de entrada 1 do diálogo
        GtkEntry *entry2 = GTK_ENTRY(dialog->entry2); // Obtém o campo de entrada 2 do diálogo

        const gchar *text1 = gtk_entry_get_text(entry1); // Obtém o texto digitado no campo de entrada 1
        const gchar *text2 = gtk_entry_get_text(entry2); // Obtém o texto digitado no campo de entrada 2

        int num1 = atoi(text1); // Converte o texto 1 para um número inteiro
        int num2 = atoi(text2); // Converte o texto 2 para um número inteiro

        if(num1 != 0 && num2 != 0){

            Appdata *app_data = (Appdata *)dialog->app_data; // Obtém a estrutura de dados da aplicação

            HistoricoGray *aux = app_data->historicogray; // Cria um ponteiro para percorrer a lista de histórico de imagens em escala de cinza

            do{
                aux = aux->prox;
            } while (aux->prox != app_data->historicogray);
            // Percorre a lista até o último elemento
            ImageGray *flipped_image = clahe_gray(aux->imageGray, num1, num2); // Aplica o CLAHE na imagem
            app_data->imagegray = flipped_image; // Atualiza a imagem atual para a imagem com o CLAHE aplicado

            app_data->historicogray = addFinalDuplamenteCircularGray(app_data->historicogray, flipped_image); // Adiciona a imagem com o CLAHE ao histórico

            GdkPixbuf *pixbuf = image_gray_to_pixbuf(flipped_image); // Converte a imagem com o CLAHE para um GdkPixbuf
            if (app_data->image_widget_gray){
                gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf); // Atualiza a imagem exibida no widget da interface
            }

        g_object_unref(pixbuf); // Libera a memória alocada para o GdkPixbuf
        }

    }
    gtk_widget_destroy(GTK_WIDGET(dialog)); // Fecha o diálogo
}

void on_button5_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *dialog; // Cria um ponteiro para o diálogo
    GtkWidget *content_area; // Cria um ponteiro para a área de conteúdo do diálogo
    GtkWindow *parent_window = GTK_WINDOW(NULL); // Obtém a janela principal

    DialogData *dialog_data = (DialogData *)g_malloc(sizeof(DialogData)); // Aloca memória para os dados do diálogo
    dialog_data->app_data = (Appdata *)user_data; // Define a estrutura de dados da aplicação nos dados do diálogo

    const char *name = "Insiras os número da caixa"; // Define o nome do diálogo

    dialog = gtk_dialog_new_with_buttons(
        name,
        parent_window,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK",
        GTK_RESPONSE_OK,
        "_Cancel",
        GTK_RESPONSE_CANCEL,
        NULL); // Cria um novo diálogo com botões de ação

    gtk_window_set_default_size(GTK_WINDOW(dialog), strlen(name) * 12, -1); // Define o tamanho padrão do diálogo
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER); // Define a posição do diálogo

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog)); // Obtém a área de conteúdo do diálogo

    GtkWidget *label1 = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do tile width",
        NULL); // Cria um novo rótulo com o texto "Digite o tamanho do tile width"

    gtk_container_add(GTK_CONTAINER(content_area), label1); // Adiciona o rótulo à área de conteúdo do diálogo
    dialog_data->entry1 = gtk_entry_new(); // Cria um novo campo de entrada
    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry1); // Adiciona o campo de entrada à área de conteúdo do diálogo

    GtkWidget *label2 = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do tile heigth",
        NULL); // Cria um novo rótulo com o texto "Digite o tamanho do tile heigth"

    gtk_container_add(GTK_CONTAINER(content_area), label2); // Adiciona o rótulo à área de conteúdo do diálogo
    dialog_data->entry2 = gtk_entry_new(); // Cria um novo campo de entrada
    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry2); // Adiciona o campo de entrada à área de conteúdo do diálogo

    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog5_response), dialog_data); // Conecta o sinal "response" do diálogo à função de callback

    gtk_widget_show_all(dialog); // Exibe o diálogo
}

void on_button6_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data; // Obtém a estrutura de dados da aplicação
    HistoricoRGB *aux = app_data->historicorgb; // Cria um ponteiro para percorrer a lista de histórico de imagens em RGB

    do{
        aux = aux->prox;
    } while (aux->prox != app_data->historicorgb);
    // Percorre a lista até o último elemento
    ImageRGB *flipped_image = flip_vertical_rgb(aux->imageRGB); // Espelha a imagem verticalmente
    app_data->imagergb = flipped_image; // Atualiza a imagem atual para a imagem espelhada

    app_data->historicorgb = addFinalDuplamenteCircularRGB(app_data->historicorgb, flipped_image); // Adiciona a imagem espelhada ao histórico

    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(flipped_image); // Converte a imagem espelhada para um GdkPixbuf
    if (app_data->image_widget_rgb){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf); // Atualiza a imagem exibida no widget da interface
    }

    g_object_unref(pixbuf); // Libera a memória alocada para o GdkPixbuf
}

void on_button7_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data; // Obtém a estrutura de dados da aplicação
    HistoricoRGB *aux = app_data->historicorgb; // Cria um ponteiro para percorrer a lista de histórico de imagens em RGB

    do{
        aux = aux->prox;
    } while (aux->prox != app_data->historicorgb);
    // Percorre a lista até o último elemento
    ImageRGB *flipped_image = flip_horizontal_rgb(aux->imageRGB); // Espelha a imagem horizontalmente
    app_data->imagergb = flipped_image; // Atualiza a imagem atual para a imagem espelhada

    app_data->historicorgb = addFinalDuplamenteCircularRGB(app_data->historicorgb, flipped_image); // Adiciona a imagem espelhada ao histórico

    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(flipped_image); // Converte a imagem espelhada para um GdkPixbuf
    if (app_data->image_widget_rgb){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf); // Atualiza a imagem exibida no widget da interface
    }

    g_object_unref(pixbuf); // Libera a memória alocada para o GdkPixbuf
}

void on_button8_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data; // Obtém a estrutura de dados da aplicação
    HistoricoRGB *aux = app_data->historicorgb; // Cria um ponteiro para percorrer a lista de histórico de imagens em RGB

    do{
        aux = aux->prox;
    } while (aux->prox != app_data->historicorgb);
    // Percorre a lista até o último elemento
    ImageRGB *flipped_image = transposeRGB(aux->imageRGB); // Transpõe a imagem
    app_data->imagergb = flipped_image; // Atualiza a imagem atual para a imagem transposta

    app_data->historicorgb = addFinalDuplamenteCircularRGB(app_data->historicorgb, flipped_image); // Adiciona a imagem transposta ao histórico

    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(flipped_image); // Converte a imagem transposta para um GdkPixbuf
    if (app_data->image_widget_rgb){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf); // Atualiza a imagem exibida no widget da interface
    }

    g_object_unref(pixbuf); // Libera a memória alocada para o GdkPixbuf
}

void on_dialog9_response(GtkDialog *dialog, gint response_id, gpointer user_data){
    if (response_id == GTK_RESPONSE_OK){
        DialogData *dialog = (DialogData *)user_data; // Obtém os dados do diálogo

        GtkEntry *entry = GTK_ENTRY(dialog->entry1); // Obtém o campo de entrada do diálogo

        const gchar *text = gtk_entry_get_text(entry); // Obtém o texto digitado no campo de entrada
        int num = atoi(text); // Converte o texto para um número inteiro

        if(num != 0){
            Appdata *app_data = (Appdata *)dialog->app_data; // Obtém a estrutura de dados da aplicação

            HistoricoRGB *aux = app_data->historicorgb; // Cria um ponteiro para percorrer a lista de histórico de imagens em RGB

            do{
                aux = aux->prox;
            } while (aux->prox != app_data->historicorgb);
            // Percorre a lista até o último elemento
            ImageRGB *flipped_image = median_blur_rgb(aux->imageRGB, num); // Aplica o filtro de mediana na imagem
            app_data->imagergb = flipped_image; // Atualiza a imagem atual para a imagem com o filtro aplicado

            app_data->historicorgb = addFinalDuplamenteCircularRGB(app_data->historicorgb, flipped_image); // Adiciona a imagem com o filtro ao histórico

            GdkPixbuf *pixbuf = image_rgb_to_pixbuf(flipped_image); // Converte a imagem com o filtro para um GdkPixbuf
            if (app_data->image_widget_rgb){
                gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf); // Atualiza a imagem exibida no widget da interface
            }

            g_object_unref(pixbuf); // Libera a memória alocada para o GdkPixbuf
        }

    }
    gtk_widget_destroy(GTK_WIDGET(dialog)); // Fecha o diálogo
}

void on_button9_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *dialog; // Cria um ponteiro para o diálogo
    GtkWidget *content_area; // Cria um ponteiro para a área de conteúdo do diálogo

    DialogData *dialog_data = (DialogData *)malloc(sizeof(DialogData)); // Aloca memória para os dados do diálogo
    dialog_data->app_data = (Appdata *)user_data; // Define a estrutura de dados da aplicação nos dados do diálogo

    GtkWindow *parent_window = GTK_WINDOW(NULL); // Obtém a janela principal

    const char *name = "Insira um número para a caixa"; // Define o nome do diálogo
    dialog = gtk_dialog_new_with_buttons(
        name,
        parent_window,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK",
        GTK_RESPONSE_OK,
        "_Cancel",
        GTK_RESPONSE_CANCEL,
        NULL); // Cria um novo diálogo com botões de ação

    gtk_window_set_default_size(GTK_WINDOW(dialog), strlen(name) * 12, -1); // Define o tamanho padrão do diálogo

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER); // Define a posição do diálogo

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog)); // Obtém a área de conteúdo do diálogo

    GtkWidget *label = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do Kernel Size",
        NULL); // Cria um novo rótulo com o texto "Digite o tamanho do Kernel Size"
 
    gtk_container_add(GTK_CONTAINER(content_area), label); // Adiciona o rótulo à área de conteúdo do diálogo
    dialog_data->entry1 = gtk_entry_new(); // Cria um novo campo de entrada

    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry1); // Adiciona o campo de entrada à área de conteúdo do diálogo

    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog9_response), dialog_data); // Conecta o sinal "response" do diálogo à função de callback

    gtk_widget_show_all(dialog); // Exibe o diálogo
}

void on_dialog10_response(GtkDialog *dialog, gint response_id, gpointer user_data){
    if (response_id == GTK_RESPONSE_OK){
        DialogData *dialog = (DialogData *)user_data; // Obtém os dados do diálogo

        GtkEntry *entry1 = GTK_ENTRY(dialog->entry1); // Obtém o campo de entrada 1 do diálogo
        GtkEntry *entry2 = GTK_ENTRY(dialog->entry2); // Obtém o campo de entrada 2 do diálogo

        const gchar *text1 = gtk_entry_get_text(entry1); // Obtém o texto digitado no campo de entrada 1
        const gchar *text2 = gtk_entry_get_text(entry2); // Obtém o texto digitado no campo de entrada 2

        if(num1 != 0 && num2 != 0){
            int num1 = atoi(text1); // Converte o texto 1 para um número inteiro
            int num2 = atoi(text2); // Converte o texto 2 para um número inteiro

            Appdata *app_data = (Appdata *)dialog->app_data; // Obtém a estrutura de dados da aplicação
            HistoricoRGB *aux = app_data->historicorgb; // Cria um ponteiro para percorrer a lista de histórico de imagens em RGB

            do{
                aux = aux->prox;
            } while (aux->prox != app_data->historicorgb);
            // Percorre a lista até o último elemento
            ImageRGB *flipped_image = clahe_rgb(aux->imageRGB, num1, num2); // Aplica o CLAHE na imagem
            app_data->imagergb = flipped_image; // Atualiza a imagem atual para a imagem com o CLAHE aplicado

            app_data->historicorgb = addFinalDuplamenteCircularRGB(app_data->historicorgb, flipped_image); // Adiciona a imagem com o CLAHE ao histórico

            GdkPixbuf *pixbuf = image_rgb_to_pixbuf(flipped_image); // Converte a imagem com o CLAHE para um GdkPixbuf
            if (app_data->image_widget_rgb){
                gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf); // Atualiza a imagem exibida no widget da interface
            }

            g_object_unref(pixbuf); // Libera a memória alocada para o GdkPixbuf
        }

    }
    gtk_widget_destroy(GTK_WIDGET(dialog)); // Fecha o diálogo
}

void on_button10_clicked(GtkButton *button, gpointer user_data){
    GtkWidget *dialog; // Cria um ponteiro para o diálogo
    GtkWidget *content_area; // Cria um ponteiro para a área de conteúdo do diálogo
    GtkWindow *parent_window = GTK_WINDOW(NULL); // Obtém a janela principal

    DialogData *dialog_data = (DialogData *)g_malloc(sizeof(DialogData)); // Aloca memória para os dados do diálogo
    dialog_data->app_data = (Appdata *)user_data; // Define a estrutura de dados da aplicação nos dados do diálogo

    const char *name = "Insiras os número da caixa"; // Define o nome do diálogo
    dialog = gtk_dialog_new_with_buttons(
        name,
        parent_window,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK",
        GTK_RESPONSE_OK,
        "_Cancel",
        GTK_RESPONSE_CANCEL,
        NULL); // Cria um novo diálogo com botões de ação

    gtk_window_set_default_size(GTK_WINDOW(dialog), strlen(name) * 12, -1); // Define o tamanho padrão do diálogo
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER); // Define a posição do diálogo

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog)); // Obtém a área de conteúdo do diálogo

    GtkWidget *label1 = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do tile width",
        NULL); // Cria um novo rótulo com o texto "Digite o tamanho do tile width"

    gtk_container_add(GTK_CONTAINER(content_area), label1); // Adiciona o rótulo à área de conteúdo do diálogo
    dialog_data->entry1 = gtk_entry_new(); // Cria um novo campo de entrada
    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry1); // Adiciona o campo de entrada à área de conteúdo do diálogo

    GtkWidget *label2 = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Digite o tamanho do tile heigth",
        NULL); // Cria um novo rótulo com o texto "Digite o tamanho do tile heigth"

    gtk_container_add(GTK_CONTAINER(content_area), label2); // Adiciona o rótulo à área de conteúdo do diálogo
    dialog_data->entry2 = gtk_entry_new();  // Cria um novo campo de entrada
    gtk_container_add(GTK_CONTAINER(content_area), dialog_data->entry2); // Adiciona o campo de entrada à área de conteúdo do diálogo

    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog10_response), dialog_data); // Conecta o sinal "response" do diálogo à função de callback

    gtk_widget_show_all(dialog); // Exibe o diálogo
}

void on_button11_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data; // Obtém a estrutura de dados da aplicação

    HistoricoGray *aux = app_data->historicogray; // Cria um ponteiro para percorrer a lista de histórico de imagens em escala de cinza

    while (aux->imageGray != app_data->imagegray){
        aux = aux->prox; // Percorre a lista até encontrar a imagem atual
    }
    app_data->imagegray = aux->ant->imageGray; // Atualiza a imagem atual para a imagem anterior

    GdkPixbuf *pixbuf = image_gray_to_pixbuf(aux->ant->imageGray); // Converte a imagem anterior para um GdkPixbuf
    if (app_data->image_widget_gray){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf); // Atualiza a imagem exibida no widget da interface
    }
}

void on_button12_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data; // Obtém a estrutura de dados da aplicação
 
    HistoricoGray *historico = app_data->historicogray; // Cria um ponteiro para percorrer a lista de histórico de imagens em escala de cinza
    HistoricoGray *aux = app_data->historicogray; // Cria um ponteiro para percorrer a lista de histórico de imagens em escala de cinza

    do{
        aux = aux->prox; 
    } while (aux->prox->imageGray != app_data->imagegray);
    // Percorre a lista até o último elemento
    app_data->historicogray = removerElementoGray(historico, app_data->imagegray);

    app_data->imagegray = aux->prox->imageGray; // Atualiza a imagem atual para a próxima imagem

    GdkPixbuf *pixbuf = image_gray_to_pixbuf(aux->prox->imageGray); // Converte a próxima imagem para um GdkPixbuf
    if (app_data->image_widget_gray){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf); // Atualiza a imagem exibida no widget da interface
    }
}

void on_button13_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data; // Obtém a estrutura de dados da aplicação
    HistoricoGray *aux = app_data->historicogray; // Cria um ponteiro para percorrer a lista de histórico de imagens em escala de cinza

    while (aux->imageGray != app_data->imagegray){
        aux = aux->prox;
    }
    app_data->imagegray = aux->prox->imageGray; // Atualiza a imagem atual para a próxima imagem

    GdkPixbuf *pixbuf = image_gray_to_pixbuf(aux->prox->imageGray); // Converte a próxima imagem para um GdkPixbuf
    if (app_data->image_widget_gray){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_gray), pixbuf); // Atualiza a imagem exibida no widget da interface
    }
}

void on_button14_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data; // Obtém a estrutura de dados da aplicação
    HistoricoRGB *aux = app_data->historicorgb; // Cria um ponteiro para percorrer a lista de histórico de imagens em RGB

    while (aux->imageRGB != app_data->imagergb){
        aux = aux->prox; // Percorre a lista até encontrar a imagem atual
    }
    app_data->imagergb = aux->ant->imageRGB; // Atualiza a imagem atual para a imagem anterior

    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(aux->ant->imageRGB); // Converte a imagem anterior para um GdkPixbuf
    if (app_data->image_widget_rgb){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf); // Atualiza a imagem exibida no widget da interface
    }
}

void on_button15_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data; // Obtém a estrutura de dados da aplicação

    HistoricoRGB *historico = app_data->historicorgb; // Cria um ponteiro para percorrer a lista de histórico de imagens em RGB
    HistoricoRGB *aux = app_data->historicorgb; // Cria um ponteiro para percorrer a lista de histórico de imagens em RGB

    do{
        aux = aux->prox;
    } while (aux->prox->imageRGB != app_data->imagergb);
    // Percorre a lista até o último elemento
    app_data->historicorgb = removerElementoRGB(historico, app_data->imagergb); // Remove a imagem atual do histórico
    
    app_data->imagergb = aux->prox->imageRGB; // Atualiza a imagem atual para a próxima imagem

    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(aux->prox->imageRGB); // Converte a próxima imagem para um GdkPixbuf
    if (app_data->image_widget_rgb){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf); // Atualiza a imagem exibida no widget da interface
    }
}

void on_button16_clicked(GtkButton *button, gpointer user_data){
    Appdata *app_data = (Appdata *)user_data; // Obtém a estrutura de dados da aplicação
    HistoricoRGB *aux = app_data->historicorgb; // Cria um ponteiro para percorrer a lista de histórico de imagens em RGB

    while (aux->imageRGB != app_data->imagergb){
        aux = aux->prox;
    }
    app_data->imagergb = aux->prox->imageRGB; // Atualiza a imagem atual para a próxima imagem

    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(aux->prox->imageRGB); // Converte a próxima imagem para um GdkPixbuf
    if(app_data->image_widget_rgb){
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_rgb), pixbuf); // Atualiza a imagem exibida no widget da interface
    }
}

void on_button17_clicked(GtkButton *button, gpointer user_data) {
    SubmitData *submit_data = (SubmitData *)user_data; // Obtém os dados do formulário de envio
    Appdata *app_data = submit_data->app_data; // Obtém a estrutura de dados da aplicação

    if (submit_data->opc == 1) {
        HistoricoRandomGray *aux = app_data->histoRandomGray; // Cria um ponteiro para percorrer a lista de histórico de imagens em escala de cinza

        while (aux->imageGray != app_data->imagerandomGray && aux->prox != NULL) {
            aux = aux->prox;
        }
        // Percorre a lista até a imagem atual
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
        }// Atualiza a imagem exibida no widget da interface
    }else if(submit_data->opc == 2){
        HistoricoRandomRGB *aux = app_data->histoRandomRGB; // Cria um ponteiro para percorrer a lista de histórico de imagens em RGB

        while (aux->imageRGB != app_data->imagerandomRGB && aux->prox != NULL) {
            aux = aux->prox;
        }
        // Percorre a lista até a imagem atual
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
        } // Atualiza a imagem exibida no widget da interface
    }
}

void on_submited_info(GtkWidget *widget, gpointer user_data) {
    SubmitData *submit_data = (SubmitData *)user_data; // Obtém os dados do formulário de envio
    Appdata *app_data = submit_data->app_data; // Obtém a estrutura de dados da aplicação
    FILE *arq; 
    HistoricoRandomGray *histoGray = NULL; 
    HistoricoRandomRGB *histoRGB = NULL;

    const gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(submit_data->file_chooser)); // Obtém o nome do arquivo selecionado

    if (filename == NULL){
        g_print("Nenhum arquivo selecionado.\n"); // Exibe uma mensagem de erro
        return;
    }

    const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(submit_data->entry)); // Obtém o texto digitado no campo de entrada
    int num = atoi(entry_text); // Converte o texto para um número inteiro
    
    srand(time(NULL)); // Inicializa a semente do gerador de números aleatórios
    if(num != 0){
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(submit_data->radio_button1))) {
            int op;
            submit_data->opc = 1;

            pngToTxT(filename, 1);
            
            arq = fopen("./utils/imagem_upload.txt", "r");
            if (!arq) {
                g_print("Erro ao abrir o arquivo.\n");
                return;
            } // Abre o arquivo para leitura

            ImageGray *image = create_image_gray(arq); // Cria uma nova imagem em escala de cinza a partir do arquivo

            ImageGray *result = image; // Cria uma nova imagem em escala de cinza para armazenar o resultado
            histoGray = lista_randon_Gray(histoGray, image); // Cria uma nova lista de histórico de imagens em escala de cinza
            
            for (int i = 0; i < num; i++) {
                op = rand() % 5;
                result = random_gray(result,op); // Aplica uma operação aleatória na imagem
                histoGray = lista_randon_Gray(histoGray, result); // Adiciona a imagem com a operação randomica ao histórico
            }

            app_data->imagerandomGray = histoGray->imageGray; // Atualiza a imagem atual para a imagem com as operações randomicas
            
            GdkPixbuf *pixbuf = image_gray_to_pixbuf(histoGray->imageGray); // Converte a imagem com as operações randomicas para um GdkPixbuf
            if (pixbuf && app_data->image_widget_random) {
                gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_random), pixbuf); // Atualiza a imagem exibida no widget da interface
            }

            if(histoGray != NULL)
                app_data->histoRandomGray = histoGray; // Atualiza o histórico de imagens em escala de cinza

        } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(submit_data->radio_button2))) {
            submit_data->opc = 2; // Define a opção selecionada como 2

            pngToTxT(filename, 0); // Converte a imagem para um arquivo de texto
            
            arq = fopen("./utils/imagem_upload.txt", "r");
            if(!arq){
                g_print("Erro ao abrir o arquivo.\n");
                return;
            }

            ImageRGB *image = create_image_rgb(arq); // Cria uma nova imagem em RGB a partir do arquivo

            histoRGB = random_RGB(image, num); // Aplica as operações randomicas na imagem
            app_data->imagerandomRGB = histoRGB->imageRGB; // Atualiza a imagem atual para a imagem com as operações randomicas

            GdkPixbuf *pixbuf = image_rgb_to_pixbuf(histoRGB->imageRGB); // Converte a imagem com as operações randomicas para um GdkPixbuf
            if(pixbuf && app_data->image_widget_random){
                gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_random), pixbuf); // Atualiza a imagem exibida no widget da interface
            }

            if(histoRGB != NULL)
                app_data->histoRandomRGB = histoRGB; // Atualiza o histórico de imagens em RGB

        }else{
            g_print("Nenhuma opção selecionada, valor de entrada: %d, arquivo: %s\n", num, filename); // Exibe uma mensagem de erro
        }

        gtk_widget_destroy(GTK_WIDGET(submit_data->window)); // Fecha a janela de diálogo
    }

}

void on_file_selected(GtkFileChooserButton *filechooserbutton, gpointer user_data) {
    SubmitData *submit_data = (SubmitData *)user_data; // Obtém os dados do formulário de envio
    Appdata *app_data = submit_data->app_data; // Obtém a estrutura de dados da aplicação
    const gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooserbutton)); // Obtém o nome do arquivo selecionado

    if(filename != NULL){
        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL); // Carrega o arquivo de imagem em um GdkPixbuf
        if (pixbuf && app_data->image_widget_random){
            gtk_image_set_from_pixbuf(GTK_IMAGE(app_data->image_widget_random), pixbuf); // Exibe a imagem no widget da interface
        }

        gtk_widget_set_sensitive(GTK_WIDGET(filechooserbutton), FALSE); // Desabilita o botão de seleção de arquivo

        GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL); // Cria uma nova janela
        gtk_window_set_title(GTK_WINDOW(window), "Insira Funções Random"); // Define o título da janela
        gtk_window_set_default_size(GTK_WINDOW(window), 400, 170); // Define o tamanho padrão da janela
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); // Define a posição da janela
        submit_data->window = window; // Define a janela de diálogo nos dados do formulário de envio

        GtkWidget *content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); // Cria uma nova área de conteúdo
        gtk_container_set_border_width(GTK_CONTAINER(content_area), 10); // Define a largura da borda da área de conteúdo
        gtk_container_add(GTK_CONTAINER(window), content_area); // Adiciona a área de conteúdo à janela

        GtkWidget *entry = gtk_entry_new(); // Cria um novo campo de entrada
        gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Digite a quantidade de operações randomicas que voce deseja fazer"); // Define o texto de orientação do campo de entrada
        gtk_box_pack_start(GTK_BOX(content_area), entry, FALSE, FALSE, 0); // Adiciona o campo de entrada à área de conteúdo
        submit_data->entry = entry; // Define o campo de entrada nos dados do formulário de envio

        GtkWidget *radio_button1 = gtk_radio_button_new_with_label(NULL, "Função Random Imagem Gray"); // Cria um novo botão de rádio
        gtk_box_pack_start(GTK_BOX(content_area), radio_button1, FALSE, FALSE, 0); // Adiciona o botão de rádio à área de conteúdo
        submit_data->radio_button1 = radio_button1; // Define o botão de rádio 1 nos dados do formulário de envio

        GtkWidget *radio_button2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_button1), "Função Random Imagem RGB");
        gtk_box_pack_start(GTK_BOX(content_area), radio_button2, FALSE, FALSE, 0); // Adiciona o botão de rádio à área de conteúdo
        submit_data->radio_button2 = radio_button2; // Define o botão de rádio 2 nos dados do formulário de envio
        
        GtkWidget *button = gtk_button_new_with_label("Iniciar"); // Cria um novo botão com o texto "Iniciar"
        g_signal_connect(button, "clicked", G_CALLBACK(on_submited_info), submit_data); // Conecta o sinal "clicked" do botão à função de callback
        gtk_box_pack_start(GTK_BOX(content_area), button, FALSE, FALSE, 0); // Adiciona o botão à área de conteúdo

        gtk_widget_show_all(window);
    }
}

GtkWidget *pagina1(gpointer user_data){
    GtkWidget *image; // Cria um ponteiro para a imagem
    Appdata *app_data = (Appdata*)user_data; // Obtém a estrutura de dados da aplicação

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0); // Cria uma nova caixa horizontal

    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); // Cria uma nova caixa horizontal
    gtk_box_pack_start(GTK_BOX(box), left_box, TRUE, TRUE, 10); // Adiciona a caixa horizontal à caixa principal
    
    ImageGray *img = (ImageGray *)app_data->historicogray->imageGray; // Obtém a imagem em escala de cinza do histórico
    app_data->imagegray = img; // Atualiza a imagem atual para a imagem em escala de cinza

    GdkPixbuf *pixbuf = image_gray_to_pixbuf(img); // Converte a imagem em escala de cinza para um GdkPixbuf

    image = gtk_image_new_from_pixbuf(pixbuf); // Cria um novo widget de imagem a partir do GdkPixbuf
    gtk_box_pack_start(GTK_BOX(left_box), image, TRUE, TRUE, 0); // Adiciona a imagem ao widget da interface

    app_data->image_widget_gray = image; // Define o widget da imagem em escala de cinza nos dados da aplicação

    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); // Cria uma nova caixa vertical
    gtk_box_pack_start(GTK_BOX(box), right_box, FALSE, TRUE, 10); // Adiciona a caixa vertical à caixa principal

    GtkWidget *titleTop = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Filtros Disponiveis",
        NULL); // Cria um novo rótulo com o texto "Filtros Disponiveis"

    gtk_container_add(GTK_CONTAINER(right_box), titleTop); // Adiciona o rótulo à caixa vertical

    GtkWidget *functions = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); // Cria uma nova caixa horizontal
    gtk_box_pack_start(GTK_BOX(right_box), functions, FALSE, FALSE, 0); // Adiciona a caixa horizontal à caixa vertical

    GtkWidget *functionGray = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15); // Cria uma nova caixa vertical
    gtk_box_pack_start(GTK_BOX(functions), functionGray, TRUE, TRUE, 0); // Adiciona a caixa vertical à caixa horizontal

    GtkWidget *button1 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Flip Vertical Gray",
        NULL); // Cria um novo botão com o texto "Flip Vertical Gray"

    GtkWidget *button2 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Flip horizontal Gray",
        NULL); // Cria um novo botão com o texto "Flip horizontal Gray"

    GtkWidget *button3 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Transpose Gray",
        NULL); // Cria um novo botão com o texto "Transpose Gray"

    GtkWidget *button4 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Media Blur Gray",
        NULL); // Cria um novo botão com o texto "Media Blur Gray"

    GtkWidget *button5 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Clahe Gray",
        NULL); // Cria um novo botão com o texto "Clahe Gray"

    GtkWidget *button11 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "<<",
        NULL); // Cria um novo botão com o texto "<<"

    GtkWidget *button12 = gtk_button_new();
    GtkWidget *lixeira = gtk_image_new_from_file("./image/lixeira.png");
    if(!gtk_image_get_pixbuf(GTK_IMAGE(lixeira))){
        g_print("Falha ao carregar a imagem\n"); // Exibe uma mensagem de erro
    }else{
        gtk_button_set_image(GTK_BUTTON(button12), lixeira); // Define a imagem do botão como a lixeira
    }

    GtkWidget *button13 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", ">>",
        NULL); // Cria um novo botão com o texto ">>"

    gtk_container_add(GTK_CONTAINER(functionGray), button1);
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), app_data); // Conecta o sinal "clicked" do botão à função de callback

    gtk_container_add(GTK_CONTAINER(functionGray), button2);
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button2_clicked), app_data); // Conecta o sinal "clicked" do botão à função de callback

    gtk_container_add(GTK_CONTAINER(functionGray), button3);
    g_signal_connect(button3, "clicked", G_CALLBACK(on_button3_clicked), app_data); // Conecta o sinal "clicked" do botão à função de callback

    gtk_container_add(GTK_CONTAINER(functionGray), button4);
    g_signal_connect(button4, "clicked", G_CALLBACK(on_button4_clicked), app_data); // Conecta o sinal "clicked" do botão à função de callback

    gtk_container_add(GTK_CONTAINER(functionGray), button5);
    g_signal_connect(button5, "clicked", G_CALLBACK(on_button5_clicked), app_data); // Conecta o sinal "clicked" do botão à função de callback 

    GtkWidget *historico = g_object_new(
        GTK_TYPE_BOX,
        "visible", TRUE,
        "orientation", GTK_ORIENTATION_VERTICAL,
        "spacing", 10,
        "homogeneous", FALSE,
        "valign", GTK_ALIGN_CENTER,
        "halign", GTK_ALIGN_CENTER,
        NULL); // Cria uma nova caixa vertical
    gtk_box_pack_start(GTK_BOX(right_box), historico, FALSE, FALSE, 0); // Adiciona a caixa vertical à caixa vertical

    GtkWidget *functionHistorico = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); // Cria uma nova caixa horizontal
    gtk_box_pack_start(GTK_BOX(historico), functionHistorico, FALSE, FALSE, 10); // Adiciona a caixa horizontal à caixa vertical

    gtk_container_add(GTK_CONTAINER(functionHistorico), button11); // Adiciona o botão 11 à caixa horizontal
    g_signal_connect(button11, "clicked", G_CALLBACK(on_button11_clicked), app_data); // Conecta o sinal "clicked" do botão à função de callback

    gtk_container_add(GTK_CONTAINER(functionHistorico), button12); // Adiciona o botão 12 à caixa horizontal
    g_signal_connect(button12, "clicked", G_CALLBACK(on_button12_clicked), app_data); // Conecta o sinal "clicked" do botão à função de callback

    gtk_container_add(GTK_CONTAINER(functionHistorico), button13); // Adiciona o botão 13 à caixa horizontal
    g_signal_connect(button13, "clicked", G_CALLBACK(on_button13_clicked), app_data); // Conecta o sinal "clicked" do botão à função de callback

    g_object_unref(pixbuf); // Libera a memória alocada para o GdkPixbuf

    return box; // Retorna a caixa principal
}

GtkWidget *pagina2(gpointer user_data){
    GtkWidget *image; // Cria um ponteiro para a imagem
    Appdata *app_data = (Appdata *)user_data; // Obtém a estrutura de dados da aplicação

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0); // Cria uma nova caixa horizontal
    
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); // Cria uma nova caixa horizontal
    gtk_box_pack_start(GTK_BOX(box), left_box, TRUE, TRUE, 10); // Adiciona a caixa horizontal à caixa principal

    ImageRGB *img = (ImageRGB *)app_data->historicorgb->imageRGB; // Obtém a imagem em RGB do histórico
    app_data->imagergb = img; // Atualiza a imagem atual para a imagem em RGB

    GdkPixbuf *pixbuf = image_rgb_to_pixbuf(img); // Converte a imagem em RGB para um GdkPixbuf

    image = gtk_image_new_from_pixbuf(pixbuf); // Cria um novo widget de imagem a partir do GdkPixbuf
    gtk_box_pack_start(GTK_BOX(left_box), image, TRUE, TRUE, 0); // Adiciona a imagem ao widget da interface

    app_data->image_widget_rgb = image; // Define o widget da imagem em RGB nos dados da aplicação

    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); // Cria uma nova caixa vertical
    gtk_box_pack_start(GTK_BOX(box), right_box, FALSE, TRUE, 10); // Adiciona a caixa vertical à caixa principal

    GtkWidget *titleTop = g_object_new(
        GTK_TYPE_LABEL,
        "visible", TRUE,
        "label", "Filtros Disponiveis",
        NULL); // Cria um novo rótulo com o texto "Filtros Disponiveis"

    gtk_container_add(GTK_CONTAINER(right_box), titleTop); // Adiciona o rótulo à caixa vertical

    GtkWidget *functions = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); // Cria uma nova caixa horizontal
    gtk_box_pack_start(GTK_BOX(right_box), functions, FALSE, FALSE, 0); // Adiciona a caixa horizontal à caixa vertical

    GtkWidget *functionRGB = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15); // Cria uma nova caixa vertical
    gtk_box_pack_start(GTK_BOX(functions), functionRGB, TRUE, TRUE, 0); // Adiciona a caixa vertical à caixa horizontal

    GtkWidget *button1 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Flip Vertical RGB",
        NULL); // Cria um novo botão com o texto "Flip Vertical RGB"

    GtkWidget *button2 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Flip horizontal RGB",
        NULL); // Cria um novo botão com o texto "Flip horizontal RGB"

    GtkWidget *button3 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Transpose RGB",
        NULL); // Cria um novo botão com o texto "Transpose RGB"

    GtkWidget *button4 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Media Blur RGB",
        NULL); // Cria um novo botão com o texto "Media Blur RGB"

    GtkWidget *button5 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "Clahe RGB",
        NULL); // Cria um novo botão com o texto "Clahe RGB"

    GtkWidget *button11 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", "<<",
        NULL); // Cria um novo botão com o texto "<<"

    GtkWidget *button12 = gtk_button_new();
    GtkWidget *lixeira = gtk_image_new_from_file("./image/lixeira.png");
    if (!gtk_image_get_pixbuf(GTK_IMAGE(lixeira))){
        g_print("Falha ao carregar a imagem\n"); // Exibe uma mensagem de erro
    }
    else{
        gtk_button_set_image(GTK_BUTTON(button12), lixeira); // Define a imagem do botão como a lixeira
    }

    GtkWidget *button13 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", ">>",
        NULL); // Cria um novo botão com o texto ">>"

    gtk_container_add(GTK_CONTAINER(functionRGB), button1);
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button6_clicked), app_data); // Conecta o sinal "clicked" do botão à função de callback

    gtk_container_add(GTK_CONTAINER(functionRGB), button2);
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button7_clicked), app_data); // Conecta o sinal "clicked" do botão à função de callback

    gtk_container_add(GTK_CONTAINER(functionRGB), button3);
    g_signal_connect(button3, "clicked", G_CALLBACK(on_button8_clicked), app_data); // Conecta o sinal "clicked" do botão à função de callback

    gtk_container_add(GTK_CONTAINER(functionRGB), button4);
    g_signal_connect(button4, "clicked", G_CALLBACK(on_button9_clicked), app_data); // Conecta o sinal "clicked" do botão à função de callback

    gtk_container_add(GTK_CONTAINER(functionRGB), button5);
    g_signal_connect(button5, "clicked", G_CALLBACK(on_button10_clicked), app_data); // Conecta o sinal "clicked" do botão à função de callback

    GtkWidget *historico = g_object_new(
        GTK_TYPE_BOX,
        "visible", TRUE,
        "orientation", GTK_ORIENTATION_VERTICAL,
        "spacing", 10,
        "homogeneous", FALSE,
        "valign", GTK_ALIGN_CENTER,
        "halign", GTK_ALIGN_CENTER,
        NULL); // Cria uma nova caixa vertical
    gtk_box_pack_start(GTK_BOX(right_box), historico, FALSE, FALSE, 0); // Adiciona a caixa vertical à caixa vertical

    GtkWidget *functionHistorico = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); // Cria uma nova caixa horizontal
    gtk_box_pack_start(GTK_BOX(historico), functionHistorico, FALSE, FALSE, 10); // Adiciona a caixa horizontal à caixa vertical

    gtk_container_add(GTK_CONTAINER(functionHistorico), button11); // Adiciona o botão 11 à caixa horizontal
    g_signal_connect(button11, "clicked", G_CALLBACK(on_button14_clicked), app_data); // Conecta o sinal "clicked" do botão à função de callback

    gtk_container_add(GTK_CONTAINER(functionHistorico), button12); // Adiciona o botão 12 à caixa horizontal
    g_signal_connect(button12, "clicked", G_CALLBACK(on_button15_clicked), app_data); // Conecta o sinal "clicked" do botão à função de callback

    gtk_container_add(GTK_CONTAINER(functionHistorico), button13); // Adiciona o botão 13 à caixa horizontal
    g_signal_connect(button13, "clicked", G_CALLBACK(on_button16_clicked), app_data); // Conecta o sinal "clicked" do botão à função de callback

    g_object_unref(pixbuf); // Libera a memória alocada para o GdkPixbuf
    return box;
}

GtkWidget *pagina0(gpointer user_data){
    Appdata *app_data = (Appdata *)user_data; // Obtém a estrutura de dados da aplicação
    SubmitData *submit_data = g_malloc(sizeof(SubmitData)); // Aloca memória para os dados do formulário de envio
    submit_data->app_data = app_data; // Define a estrutura de dados da aplicação nos dados do formulário de envio

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0); // Cria uma nova caixa horizontal

    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); // Cria uma nova caixa horizontal
    gtk_box_pack_start(GTK_BOX(box), left_box, TRUE, TRUE, 10); // Adiciona a caixa horizontal à caixa principal

    GtkWidget *image = gtk_image_new(); // Cria um novo widget de imagem
    gtk_box_pack_start(GTK_BOX(left_box), image, TRUE, TRUE, 0); // Adiciona a imagem ao widget da interface
    app_data->image_widget_random = image; // Define o widget da imagem aleatória nos dados da aplicação

    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); // Cria uma nova caixa vertical
    gtk_box_pack_start(GTK_BOX(box), right_box, FALSE, TRUE, 10); // Adiciona a caixa vertical à caixa principal

    GtkWidget *spacer_top = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); // Cria uma nova caixa vertical
    gtk_box_pack_start(GTK_BOX(right_box), spacer_top, TRUE, TRUE, 0); // Adiciona a caixa vertical à caixa vertical

    GtkWidget *file_chooser = g_object_new(
        GTK_TYPE_FILE_CHOOSER_BUTTON,
        "title", "Selecionar Imagem",
        "action", GTK_FILE_CHOOSER_ACTION_OPEN,
        "visible", TRUE,
        NULL
    ); // Cria um novo seletor de arquivo
    g_signal_connect(G_OBJECT(file_chooser), "file-set", G_CALLBACK(on_file_selected), submit_data); // Conecta o sinal "file-set" do seletor de arquivo à função de callback
    gtk_box_pack_start(GTK_BOX(right_box), file_chooser, FALSE, FALSE, 0); // Adiciona o seletor de arquivo à caixa vertical
    submit_data->file_chooser = file_chooser; // Define o seletor de arquivo nos dados do formulário de envio

    GtkWidget *button1 = g_object_new(
        GTK_TYPE_BUTTON,
        "visible", TRUE,
        "label", ">>",
        NULL
    ); // Cria um novo botão com o texto ">>"
    gtk_container_add(GTK_CONTAINER(right_box), button1); // Adiciona o botão à caixa vertical
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button17_clicked), submit_data); // Conecta o sinal "clicked" do botão à função de callback

    GtkWidget *spacer_bottom = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); // Cria uma nova caixa vertical
    gtk_box_pack_start(GTK_BOX(right_box), spacer_bottom, TRUE, TRUE, 0); // Adiciona a caixa vertical à caixa vertical

    gtk_widget_set_halign(file_chooser, GTK_ALIGN_CENTER); // Define o alinhamento horizontal do seletor de arquivo como central

    return box;
}

GtkWidget *criaPaginas(gpointer user_data){
    GtkWidget *stack = g_object_new(
        GTK_TYPE_STACK,
        "visible", TRUE,
        "transition-type", GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT,
        NULL); // Cria um novo stack

    GtkWidget *page0 = pagina0(user_data); // Cria a página 0
    GtkWidget *page1 = pagina1(user_data); // Cria a página 1
    GtkWidget *page2 = pagina2(user_data); // Cria a página 2

    gtk_stack_add_titled(GTK_STACK(stack), page0, "page0", "Page Inicial"); // Adiciona a página 0 ao stack
    gtk_stack_add_titled(GTK_STACK(stack), page1, "page1", "Image Gray"); // Adiciona a página 1 ao stack
    gtk_stack_add_titled(GTK_STACK(stack), page2, "page2", "Image RGB"); // Adiciona a página 2 ao stack

    return stack;
}

void app_activate(GApplication *app, gpointer user_data){
    GtkWidget *window; // Cria um ponteiro para a janela
    GtkWidget *switcher; // Cria um ponteiro para o switcher
    GtkWidget *pages; // Cria um ponteiro para as páginas

    window = gtk_application_window_new(GTK_APPLICATION(app)); // Cria uma nova janela
    gtk_window_set_title(GTK_WINDOW(window), "SWJ"); // Define o título da janela
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600); // Define o tamanho padrão da janela
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); // Define a posição da janela

    // Cria a Box da Janela
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); // Cria uma nova caixa vertical

    pages = criaPaginas(user_data); // Cria as páginas

    switcher = g_object_new(
        GTK_TYPE_STACK_SWITCHER,
        "visible", TRUE,
        "stack", pages,
        "halign", GTK_ALIGN_END,
        NULL); // Cria um novo switcher
    gtk_box_pack_start(GTK_BOX(box), pages, TRUE, TRUE, 0); // Adiciona as páginas à caixa
    gtk_box_pack_start(GTK_BOX(box), switcher, FALSE, TRUE, 0);  // Adiciona o switcher à caixa

    gtk_container_add(GTK_CONTAINER(window), box); // Adiciona a caixa à janela
    gtk_widget_show_all(window); // Exibe todos os widgets da janela
}
