#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mandelbrot.h"

typedef struct{
    const Config *cfg;
    int *image;
    int start_row;
    int end_row;
} ThreadArgs1;

static void *worker1(void *arg){
    ThreadArgs1 *targs = (ThreadArgs1 *)arg;
    const Config *cfg = targs -> cfg;
    double re_step = (RE_MAX - RE_MIN) / cfg -> width;
    double im_step = (IM_MAX - IM_MIN) / cfg -> height;

    for(int y = targs -> start_row; y < targs -> end_row; y++){
        double c_imag = IM_MIN + y * im_step;
        for(int x = 0; x < cfg->width; x++){
            double c_real = RE_MIN + x * re_step;
            targs -> image[y * cfg->width + x] = mandelbrot_iterations(c_real, c_imag, cfg->max_iter);
        }
    }
    return NULL;
}

int compute_threads1(const Config *cfg, int *image){
    int n = cfg->num_threads;
    pthread_t *threads = malloc(n * sizeof(pthread_t));
    ThreadArgs1 *targs = malloc(n * sizeof(ThreadArgs1));
    if(threads == NULL || targs == NULL){
        fprintf(stderr, "Erro!! Falha ao alocar memória para threads (pthreads1)\n");
        free(threads);
        free(targs);
        return -1;
    }

    int rows_per_thread = cfg -> height / n;
    int remainder = cfg -> height % n;
    int next_row = 0;

    for(int t = 0; t < n; t++){
        int rows = rows_per_thread + (t < remainder ? 1 : 0);
        targs[t].cfg = cfg;
        targs[t].image = image;
        targs[t].start_row = next_row;
        targs[t].end_row = next_row + rows;
        next_row += rows;

        if(pthread_create(&threads[t], NULL, worker1, &targs[t]) != 0){
            fprintf(stderr, "Erro!! falha ao ciar thread %d (pthreads1)\n", t);
            for(int j = 0; j < t; j++) pthread_join(threads[j], NULL);
            free(threads);
            free(targs);
            return -1;
        }
    }

    for(int t = 0; t < n; t++){
        pthread_join(threads[t], NULL);
    }

    free(threads);
    free(targs);
    return 0;
}