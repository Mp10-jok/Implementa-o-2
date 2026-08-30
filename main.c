#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mandelbrot.h"

#define LOGIN "mrpf"

static int parse_positive_int(const char *str, int *out){
    char *endptr;
    long val = strtol(str, &endptr, 10);
    if(*str == '\0' || *endptr != '\0' || val <= 0 || val > 100000){
        return -1;
    }
    *out = (int)val;
    return 0;
}

static double elapsed_seconds(struct timespec start, struct timespec end){
    return (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;
}

static void free_all(int *a, int *b, int *c, int *d){
    free(a); free(b); free(c); free(d);
}

int main(int argc, char *argv[]){
    if(argc != 5){
        fprintf(stderr, "Uso: %s [largura] [altura] [max_iteracoes] [num_threads]\n", argv[0]);
        return 1;
    }

    Config cfg;
    if(parse_positive_int(argv[1], &cfg.width) != 0 || parse_positive_int(argv[2], &cfg.height) != 0 || parse_positive_int(argv[3], &cfg.max_iter) != 0 || parse_positive_int(argv[4], &cfg.num_threads) != 0){
        fprintf(stderr, "Erro!! Todos os valores devem ser positivos inteiros\n");
        return 1;
    }

    int *img_serial = alloc_image(cfg.width, cfg.height);
    int *img_openmp = alloc_image(cfg.width, cfg.height);
    int *img_pth1 = alloc_image(cfg.width, cfg.height);
    int *img_pth2 = alloc_image(cfg.width, cfg.height);

    if(!img_serial || !img_openmp || !img_pth1 || !img_pth2){
        free_all(img_serial, img_openmp, img_pth1, img_pth2);
        return 1;
    }

    FILE *times_file = fopen("times.txt", "w");
    if(times_file == NULL){
        fprintf(stderr, "Eroo!! Não foi possível criar o times.txt\n");
        free_all(img_serial, img_openmp, img_pth1, img_pth2);
        return 1;
    }

    struct timespec t0, t1;

    clock_gettime(CLOCK_MONOTONIC, &t0);
    compute_serial(&cfg, img_serial);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    fprintf(times_file, "serial: %.6f s\n", elapsed_seconds(t0, t1));

    clock_gettime(CLOCK_MONOTONIC, &t0);
    compute_openmp(&cfg, img_openmp);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    fprintf(times_file, "openmp: %.6f s\n", elapsed_seconds(t0, t1));

    clock_gettime(CLOCK_MONOTONIC, &t0);
    if(compute_pthreads1(&cfg, img_pth1) != 0){
        fclose(times_file);
        free_all(img_serial, img_openmp, img_pth1, img_pth2);
        return 1;
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    fprintf(times_file, "pthreads1: %.6f s\n", elapsed_seconds(t0, t1));

    clock_gettime(CLOCK_MONOTONIC, &t0);
    if(compute_pthreads2(&cfg, img_pth2) != 0){
        fclose(times_file);
        free_all(img_serial, img_openmp, img_pth1, img_pth2);
        return 1;
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    fprintf(times_file, "pthreads2: %.6f s\n", elapsed_seconds(t0, t1));

    fclose(times_file);

    normalize_image(img_serial, cfg.width, cfg.height, cfg.max_iter);
    normalize_image(img_openmp, cfg.width, cfg.height, cfg.max_iter);
    normalize_image(img_pth1, cfg.width, cfg.height, cfg.max_iter);
    normalize_image(img_pth2, cfg.width, cfg.height, cfg.max_iter);

    char filename[256];
    int ok = 0;

    snprintf(filename, sizeof(filename), "mandelbrot_%s_serial.pgm", LOGIN);
    ok |= write_pgm(filename, img_serial, cfg.width, cfg.height);

    snprintf(filename, sizeof(filename), "mandelbrot_%s_openmp.pgm", LOGIN);
    ok |= write_pgm(filename, img_openmp, cfg.width, cfg.height);

    snprintf(filename, sizeof(filename), "mandelbrot_%s_pthreads1.pgm", LOGIN);
    ok |= write_pgm(filename, img_pth1, cfg.width, cfg.height);

    snprintf(filename, sizeof(filename), "mandelbrot_%s_pthreads2.pgm", LOGIN);
    ok |= write_pgm(filename, img_pth2, cfg.width, cfg.height);

    free_all(img_serial, img_openmp, img_pth1, img_pth2);

    return ok == 0 ? 0 : 1;
}