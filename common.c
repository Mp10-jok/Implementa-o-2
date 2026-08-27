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