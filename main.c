#include <stdio.h>
#include <stdlib.h>

#include "./functions/image.c"

int main(){
    FILE *arq;
    arq = fopen("../utils/input_image_example_RGB.txt","r");
    
    ImageRGB* imagergb = create_image_rgb(arq);

    ImageRGB* clahe = clahe_rgb(imagergb,256,256);

    mostrar_imagem_RGB(imagergb);
    mostrar_imagem_RGB(clahe);

    free_image_rgb(clahe);
    free_image_rgb(imagergb);

    // for(int i=1;i<512;i++){
    //     if(512 % i == 0){
    //         printf("%d\n",i);
    //     }
    // }
    
    return 0;
}