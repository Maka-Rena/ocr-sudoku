#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../libraries/matrix_lib/include/matrix.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./include/hough_transform.h"
#include "./include/black_n_white.h"
#include "./include/cell.h"
#include "./include/contrast_light.h"
#include "./include/crop_ia.h"
#include "./include/crop.h"
#include "./include/gaussian.h"
#include "./include/grayscale.h"
#include "./include/resize.h"
#include "./include/sobel.h"
#include "./include/to_matrix.h"

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* Load_image(const char* path)
{
    SDL_Surface* surface = IMG_Load(path);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface* res = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGB888,0);
    if (res == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(surface);
    return res;
}

int main(int argc, char** argv)
{
    if (argc == 1)
        errx(EXIT_FAILURE, "%s", "no arg");

    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a surface from the colored image.
    SDL_Surface* surface = Load_image(argv[1]);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Convert the surface into grayscale.
    Surface_To_Grayscale(surface);
    IMG_SavePNG(surface, "grayscale.png");

    // - Convert the surface into blurred surface
	Kernel_Convolution(surface);
    IMG_SavePNG(surface, "blurred.png");

    blackandwhite(surface);

    // - Convert the surface into sobel surface 
	Kernel_Convolution_Sobel(surface);
    IMG_SavePNG(surface, "sobel.png");

    Image *image = CV_SURFACE_TO_IMG(surface);
    if (image == NULL)
        errx(1, "Failed converting the surface to image");

    //Hough Transform call
    int n = 0;
    int *lines = CV_HOUGH_LINES(image, 300, &n);
    int *merged = CV_MERGE_LINES(lines, n, 50, &n);
    CV_DRAW_LINES(image, image, merged, n, 2, CV_RGB(255, 0, 0));

    // - Save the image.
    surface = CV_IMG_TO_SURFACE(image);
    IMG_SavePNG(surface, "lines.png");

    // - Free the surface.
    SDL_FreeSurface(surface);
    CV_FREE(&image);

    free(lines);
    free(merged);
    return EXIT_SUCCESS;
}