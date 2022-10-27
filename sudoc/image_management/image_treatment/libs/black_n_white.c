#include "../include/black_n_white.h"


void blackandwhite(SDL_Surface* surface)
{
    int length = surface->w * surface->h;

 
    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_LockSurface(surface); 

    int count = 0;
    while (count < length)
    {
        Uint8 r,g,b;
		SDL_GetRGB(pixels[count], format, &r, &g, &b);

        if (((r+b+g)/3) >= 127)
        {

            pixels[count] = SDL_MapRGB(format, 255,255,255);
        }
        else
        {
            pixels[count] = SDL_MapRGB(format, 0, 0, 0);
        }
        count++;
    }

}