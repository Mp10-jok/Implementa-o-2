CC = gcc
CFLAGS = -Wall -Wextra -02 -std=c11 -fopenmp
LDFLAGS = -lpthread -lm -fopenmp

SRC = main.c common.c serial.c openmp_impl.c pthreads1.c pthreads2.c
OBJ = $(SRC:.c=.o)
TARGET = mandelbrot

all: $(TARGET)

$(TARGET): $(OBJ)
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.c mandelbrot.h
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJ) $(TARGET) *.pgm times.txt

.PHONY: all clean