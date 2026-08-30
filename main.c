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
