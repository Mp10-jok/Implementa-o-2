#include "mandelbrot.h"

void compute_serial(const Config *cfg, int *image){
    double re_step = (RE_MAX - RE_MIN) / cfg -> width;
    double im_step = (IM_MAX - IM_MIN) / cfg -> height;

    for(int y = 0; y < cfg -> height; y++){
        double c_imag = IM_MIN + y * im_step;
        for(int x = 0; x < cfg -> width; x++){
            double c_real = RE_MIN + x * re_step;
            image[y * cfg -> width + x] = mandelbrot_iterations(c_real, c_imag, cfg -> max_iter);
        }
    }
}