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