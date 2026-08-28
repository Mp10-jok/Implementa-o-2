#include <stdio.h>
#include <stdlib.h>
#include "mandelbrot.h"

int mandelbrot_iterations(double c_real, double c_imag, int max_iter){
    double z_real = 0.0, z_imag = 0.0;
    int iter = 0;

    while(iter < max_iter && (z_real * z_real + z_imag * z_imag) <= 4.0){
        double new_real = z_real * z_real - z_imag * z_imag + c_real;
        double new_imag = 2.0 * z_real * z_imag + c_imag;
        z_real = new_real;
        z_imag = new_imag;
        iter++;
    }
    return iter;
}

int *alloc_image(int width, int height){
    int *image = malloc((size_t)width * (siz_t)height * sizeof(int));
    if(image == NULL){
        fprintf(stderr, "Erro ao alocar memoria pra imagem (%dx%d)\n", width, height);
    }
    return image;
}

void normalize_image(int *image, int width, int height, int max_iter){
    for(int i = 0; i < width * height; i++){
        image[i] = (int)((double)image[i] / (double)max_iter *255.0);
    }
}

int write_pgm(const char *filename, const int *image, int width, int height){
    FILE *f = fopen(filename, "w");
    if(f == NULL){
        fprintf(stderr, "Erro!! O arquivo de saída nao foi criado '%s'\n", filename);
        return -1;
    }

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            fprintf(f, "%d", image[y * width + x]);
            if(x < width - 1) fputc(' ', f);
        }
        fputc('\n', f);
    }

    fclose(f);
    return 0;
}
void registrar_evidencia(const char *mensagem){
    FILE *arquivo = fopen("evidencias.log", "a");
    if(arquivo == NULL) return;
    fprintf(arquivo, "%s\n", mensagem);
    fclose(arquivo);
}
void registrar_tempo(const char *implementacao, double tempo){
    FILE *arquivo = fopen("times.txt", "a");
    if(arquivo == NULL) return;
    fprintf(arquivo, "%s: %.6f segundos\n", implementacao, tempo);
    fclose(arquivo);
}