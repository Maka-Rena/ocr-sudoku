#include "../include/contrast_light.h"


void contrast(SDL_Surface* surface)
{
    int length = surface->w * surface->h;
    Uint32* pixels = surface->pixels;

    for(int i = 0; i < length; i++)
    {
        Uint8 r,g,b;
        SDL_GetRGB(pixels[i], surface->format, &r, &g, &b);
        double newcolor = r*0.1 + r;
        if (newcolor > 255)
            newcolor = 255;
        pixels[i] = SDL_MapRGB(surface->format, newcolor, newcolor, newcolor);
    }
}