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