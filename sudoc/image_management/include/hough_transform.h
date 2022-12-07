#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef float pixel_t;

typedef struct
{
    int c;
    int h;
    int w;
    pixel_t *data;
} Image;

#define FREE(ptr)       \
    do                  \
    {                   \
        if (ptr)        \
        {               \
            free(ptr);  \
            ptr = NULL; \
        }               \
    } while (0)

#define PIXEL(image, c, i, j) \
    (image->data[(c) * (image)->h * (image)->w + (i) * (image)->w + (j)])

#define min(a, b) ((a) < (b) ? (a) : (b))

#define max(a, b) ((a) > (b) ? (a) : (b))

#define norm(x) min(max((x), 0), 1)

void CV_FREE(Image **image);
Image *CV_INIT(int channels, int height, int width);
Image *CV_COPY(const Image *src);
Image *CV_SURFACE_TO_IMG(SDL_Surface *surface);
SDL_Surface *CV_IMG_TO_SURFACE(const Image *image);
Uint32 CV_RGB(Uint8 r, Uint8 g, Uint8 b);

int *CV_HOUGH_LINES(const Image *src, int threshold, int *nlines);
int *CV_MERGE_LINES(int *lines, int nlines, int threshold, int *n);
Image *CV_DRAW_POINT(const Image *src, Image *dst, int x, int y, int width, Uint32 color);
Image *CV_DRAW_LINE(const Image *src, Image *dst, int x1, int y1, int x2, int y2, int width, Uint32 color);
Image *CV_DRAW_LINES(const Image *src, Image *dst, int *lines, int nlines, int weight, Uint32 color);