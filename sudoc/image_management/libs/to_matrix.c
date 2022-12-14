#include "../include/to_matrix.h"

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* tempsurface = IMG_Load(path);
    if (tempsurface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tempsurface, SDL_PIXELFORMAT_RGB888, 0);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_FreeSurface(tempsurface);
    return surface;
}

// Transform the pixel in an int : 0 for black end 1 for white.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
int pixel_to_bin(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b); 
    Uint8 average = (r + g + b) / 3;
    if (average < 50)
        return 0;
    else 
        return 1;
}

Matrix* surface_to_matrix(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    Matrix* matrix = matrix_create(surface->w,surface->h);
    SDL_LockSurface(surface);
    for(int i = 0 ; i < matrix-> rows ; i++)
        for(int j = 0 ; j < matrix -> cols ; j++)
        {
            SDL_PixelFormat* format = surface->format;
            matrix -> entries[i][j] = pixel_to_bin(pixels[i*matrix -> rows + j],format);
        }
    SDL_UnlockSurface(surface);
    return matrix;
}

