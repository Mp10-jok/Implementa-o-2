#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mandelbrot.h"

typedef struct{
    const Config *cfg;
    int *image;
    int start_now;
    int end_now;
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

