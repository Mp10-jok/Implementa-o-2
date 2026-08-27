#ifndef MANDELBROT_H
#define MANDELBROT_H
#define RE_MIN -2.0
#define RE_MAX 1.0
#define IM_MIN -1.5
#define IM_MAX 1.5

typedef struct{
    int width;
    int height;
    int max_iter;
    int num_threads;
} Config;

int mandelbrot_iterations(double c_real, double c_imag, int max_iter);
int *alloc_image(int width, int height);
void normalize_image(int *image, int width, int height, int max_iter);
int write_pgm(const char *filename, const int *image, int width, int height);

void compute_serial(const Config *cfg, int *image);
void compute_openmp(const Config *cfg, int *image);
int compute_pthreads1(const Config *cfg, int *image);
int compute_pthreads2(const Config *cfg, int *image);

#endif