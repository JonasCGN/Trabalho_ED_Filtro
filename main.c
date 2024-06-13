#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

#include "./functions/image.h"
#include "./interface/tela.h"

int main(int argc, char **argv){
    HistoricoGray *histoGray = criaListaGray();
    HistoricoRGB *histoRGB = criaListaRGB();
    
    FILE *arq;
    arq = fopen("./utils/input_image_example_Gray.txt","r");
    ImageRGB *imagegray = create_image_gray(arq);

    histoGray = addFinalDuplamenteCircularGray(histoGray,imagegray);

    arq = fopen("./utils/input_image_example_RGB.txt","r");
    ImageRGB *imagergb = create_image_rgb(arq);

    histoRGB = addFinalDuplamenteCircularRGB(histoRGB,imagergb);

    Appdata *app_data = {histoRGB,histoGray,NULL,NULL};

    GtkApplication *app;
    int status;

    app = gtk_application_new("com.swj.manipulationimage", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), &app_data);

    // Run the application
    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    g_free(app_data->historicorgb);
    g_free(app_data->historicogray);
    // g_free(app_data);

    return status;
}