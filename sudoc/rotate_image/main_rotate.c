#include "./include/rotate.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char **argv)
{
    if (argc != 4)
        errx(EXIT_FAILURE, "Usage: %s <image> <output>", argv[0]);

    SDL_Surface *image = IMG_Load(argv[1]);
    //Create rotated surface with the same size as the original
    SDL_Surface *rotated = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);
    SDL_FillRect(rotated, NULL, SDL_MapRGB(rotated->format, 255, 255, 255));


    //Convert the pixel format to RGBA32
    image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA32, 0);

    if (image == NULL)
        errx(EXIT_FAILURE, "Error: %s\n", SDL_GetError());

    //Rotate the image
    rotate_image(image, rotated, atof(argv[3]));
    IMG_SavePNG(rotated, "out.png");
    return EXIT_SUCCESS;
}
