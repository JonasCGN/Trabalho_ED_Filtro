#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

#include "./functions/image/image.h"
#include "./functions/lista/list.h"
#include "./functions/interface/tela.h"

int main(int argc, char **argv){
    HistoricoGray *histoGray = criaListaGray();
    HistoricoRGB *histoRGB = criaListaRGB();
    
    FILE *arq;
    // arq = fopen("./utils/leao_gray.txt","r");
    arq = fopen("./utils/input_image_example_Gray.txt","r");
    if(!arq){
        printf("Não foi possivel abrir o arquivo");
        exit(1);
    }
    ImageGray *imagegray = create_image_gray(arq);
    histoGray = addFinalDuplamenteCircularGray(histoGray,imagegray);

    // arq = fopen("./utils/leao_rgb.txt","r");
    arq = fopen("./utils/input_image_example_RGB.txt","r");
    if(!arq){
        printf("Não foi possivel abrir o arquivo");
        exit(1);
    }
    ImageRGB *imagergb = create_image_rgb(arq);
    histoRGB = addFinalDuplamenteCircularRGB(histoRGB,imagergb);

    Appdata app_data = {histoRGB,histoGray,NULL,NULL,NULL,NULL,NULL,NULL};

    GtkApplication *app;
    int status;

    app = gtk_application_new("com.swj.manipulationimage", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), &app_data);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    //Dando erro aqui
    liberaListaRGB(app_data.historicorgb,app_data.historicorgb);
    liberaListaGray(app_data.historicogray,app_data.historicogray);
    
    liberaImageRGB(app_data.imagergb);
    liberaImageGray(app_data.imagegray);
    

    return status;
}
