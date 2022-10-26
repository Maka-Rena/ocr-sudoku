#include "../include/grayscale.h"

// Converts a colored pixel into grayscale.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
void __Pixel_To_Grayscale(Uint32 pixel_color, SDL_PixelFormat* format, Uint32 *color, uint *average)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    
    *average = 0.3*r + 0.59*g +0.11*b;
    *color = SDL_MapRGB(format, *average, *average, *average);
}

void Surface_To_Grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    int len = surface->w * surface->h;
    
    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_LockSurface(surface);
    int count = 0;
    
    
    while (count < len)
    {
        Uint32 color;
        uint average;
        __Pixel_To_Grayscale(pixels[count], format, &color, &average);
        
        pixels[count] = color;
        count++;
    }

    SDL_UnlockSurface(surface);
}


