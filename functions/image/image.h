#ifndef IMAGE_H
#define IMAGE_H

typedef struct dimensoes
{
    int altura, largura;
} Dimensoes;

typedef struct pixelRGB
{
    int red, blue, green;
} PixelRGB;

typedef struct pixelGray
{
    int value;
} PixelGray;

typedef struct imageGray
{
    Dimensoes dim;
    PixelGray *pixels;
} ImageGray;

typedef struct imageRGB
{
    Dimensoes dim;
    PixelRGB *pixels;
} ImageRGB;

// Funções de criação e liberação
ImageGray *create_image_gray(FILE *file);
void mostrar_imagem_RGB(ImageRGB *img);
void free_image_gray(ImageGray *image);

ImageRGB *create_image_rgb(FILE *arq);
void mostrar_imagem_Gray(ImageGray *img);
void free_image_rgb(ImageRGB *image);

// Operações para ImageGray
ImageGray *flip_vertical_gray(ImageGray *image);
ImageGray *flip_horizontal_gray(ImageGray *image);
ImageGray *transposeGray(const ImageGray *image);


// Operações para ImageRGB
ImageRGB *flip_vertical_rgb(const ImageRGB *image);
ImageRGB *flip_horizontal_rgb(const ImageRGB *image);
ImageRGB *transposeRGB(const ImageRGB *image);

// Manipulação por pixel para ImageGray
ImageGray *clahe_gray(const ImageGray *image, int tile_width, int tile_height);
ImageGray *median_blur_gray(const ImageGray *image, int kernel_size);

// Manipulação por pixel para ImageRGB
ImageRGB *clahe_rgb(const ImageRGB *image, int tile_width, int tile_height);
ImageRGB *median_blur_rgb(const ImageRGB *image, int kernel_size);
#endif // IMAGE_H