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

