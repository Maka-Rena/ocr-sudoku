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
} Images;

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

void CV_FREE(Images **image);
Images *CV_INIT(int channels, int height, int width);
Images *CV_COPY(const Images *src);
Images *CV_SURFACE_TO_IMG(SDL_Surface *surface);
SDL_Surface *CV_IMG_TO_SURFACE(const Images *image);
Uint32 CV_RGB(Uint8 r, Uint8 g, Uint8 b);

int *Hough_lines(const Images *src, int threshold, int *nlines);
int *Hough_merge_lines(int *lines, int nlines, int threshold, int *n);
Images *Hough_draw_point(const Images *src, Images *dst, int x, int y, int width, Uint32 color);
Images *Hough_draw_line(const Images *src, Images *dst, int x1, int y1, int x2, int y2, int width, Uint32 color);
Images *Hough_draw_lines(const Images *src, Images *dst, int *lines, int nlines, int weight, Uint32 color);
Images *Hough_draw_circle(const Images *src, Images *dst, int x, int y, int r, int width, Uint32 color);

float Find_orientation(int *lines, int nlines);

int *Compute_intersections(int *lines, int nlines, int *nintersection, int width, int height, int ymin, int ymax);
int *Sort_intersections(int *intersections, int nintersections);
int *Find_intersections(int *lines, int nlines, int *nintersection);
int *Get_grid(int *intersections, int nintersections);