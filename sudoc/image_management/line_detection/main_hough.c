#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./include/hough_transform.h"

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
    Image *image = CV_SURFACE_TO_IMG(surface);
    if (image == NULL)
        errx(1, "Failed converting the surface to image");

    int n = 0;
    int *lines = CV_HOUGH_LINES(image, 300, &n);
    int *merged = CV_MERGE_LINES(lines, n, 50, &n);

    CV_DRAW_LINES(image, image, merged, n, 2, CV_RGB(255, 0, 0));

    surface = CV_IMG_TO_SURFACE(image);
    IMG_SavePNG(surface, "result.png");
    // - Free the surface.
    SDL_FreeSurface(surface);
    CV_FREE(&image);

    free(lines);
    free(merged);
    return EXIT_SUCCESS;
}