from PIL import Image
import numpy as np

def txt_to_jpg(txt_file_path, jpg_file_path):
    with open(txt_file_path, 'r') as file:
        altura = int(file.readline().strip())
        largura = int(file.readline().strip())
        
        image_data = np.zeros((altura, largura, 3), dtype=np.uint8)
        
        for i in range(altura):
            line = file.readline().strip()
            pixels = line.split(',')
            for j in range(largura):
                r, g, b = map(int, pixels[j].split())
                image_data[i, j] = [r, g, b]

    image = Image.fromarray(image_data)
    image.save(jpg_file_path)

def transformaImagemTXT(text_path,out_path):
    imagem = Image.open(text_path)

    imagem = imagem.convert('RGB')

    with open(out_path, 'w') as arquivo:
        
        largura, altura = imagem.size
        arquivo.write(f'{altura}\n{largura}\n')
        
        for y in range(altura):
            for x in range(largura):
                r, g, b = imagem.getpixel((x, y))
                arquivo.write(f'{r} {g} {b},')
            arquivo.write('\n')

# transformaImagemTXT('nagant.jpg','nagant.txt')
txt_to_jpg('lulut.txt','lena.jpg')
txt_to_jpg('lulu.txt','lena2.jpg')