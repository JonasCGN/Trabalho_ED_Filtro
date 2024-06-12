#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

#include "./functions/image.h"
#include "./interface/tela.h"

int main(int argc, char **argv){
    HistoricoGray *histoGray = criaListaGray();
    HistoricoRGB *histoRGB = criaListaRGB();

    Appdata appdata = {histoRGB,histoGray};

    GtkApplication *app;
    int status;

    app = gtk_application_new("com.swj.manipulationimage", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), &appdata);

    // Run the application
    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}
