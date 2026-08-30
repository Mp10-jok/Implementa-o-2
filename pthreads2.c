#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mandelbrot.h"

typedef struct{
    const Config *cfg;
    int image;
    int *next_row;
    pthread_mutex_t *mutex;
} ThreadsArgs2;

static void *worker2(void *arg){
    ThreadsArgs2 *targs = (ThreadsArgs2 *)arg;
    const Config *cfg = targs -> cfg;
    double re_step = (RE_MAX - RE_MIN) / cfg -> width;
    double im_step = (IM_MAX - IM_MIN) / cfg -> height;

    while(1){
        int y;

        pthread_mutex_lock(targs -> mutex);
        y = *targs -> next_row;
        if(y < cfg -> height){
            (*targs -> next_row)++;
        }
        pthread_mutex_unlock(targs -> mutex);

        if(y >= cfg -> height) break;

        double c_imag = IM_MIN + y * im_step;
        for(int x = 0; x < cfg -> width; x++){
            double c_real = RE_MIN + x * re_step;
            targs -> image[y * cfg -> width + x] = mandelbrot_iterations(c_real, c_imag, cfg -> max_iter);
        }
    }
    return NULL;
}

int compute_pthreads2(const Config *cfg, int *image){
    int n = cfg -> num_threads;
    pthread_t *threads = malloc(n * sizeof(pthread_t));
    ThreadsArgs2 * targs = malloc(n * sizeof(ThreadsArgs2));
    int next_row = 0;
    pthread_mutex_t mutex;

    if(threads == NULL || targs == NULL){
        fprintf(stderr, "Erro!! Falha ao alocar memoria para threads (pthreads2)\n");
        free(threads);
        free(targs);
        return -1;
    }

    pthread_mutex_init(&mutex, NULL);

    for(int t = 0; t < n; t++){
        targs[t].cfg = cfg;
        targs[t].image = image;
        targs[t].next_row = &next_row;
        targs[t].mutex = &mutex;

        if(pthread_create(&threads[t], NULL, worker2, &targs[t]) != 0){
            fprintf(stderr, "Erro!! Falha ao criar thread %d (pthreads2)\n", t);
            for(int j = 0; j < t; j++) pthread_join(threads[j], NULL);
            pthread_mutex_destroy(&mutex);
            free(threads);
            free(targs);
            return -1;
        }
    }

    for(int t = 0; t < n; t++){
        pthread_join(threads[t], NULL);
    }

    pthread_mutex_destroy(&mutex);
    free(threads);
    free(targs);
    return 0;
}