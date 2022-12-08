#include "../include/contrast_light.h"


void contrast(SDL_Surface* surface)
{
    //correction of the light
    int length = surface->w * surface->h;
    int factor = 10;

    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_LockSurface(surface);
    int count = 0;
    int hist[256] = { 0 };

    while (count < length)
    {
        int i = 0;
        int found = 1;
        while (i < factor && found)
        {
            uint upperbound = (i+1)*(255/factor);
            uint lowerbound = i*(255/factor);
            if (pixels[count] > lowerbound && pixels[count] < upperbound)
            {
                pixels[count] = SDL_MapRGB(format, upperbound, upperbound, upperbound);
                i++;
            }
            else
            {
                i++;
            }
            
            //step1 for equalization : get the histogram 
            Uint8 r, g, b;
            SDL_GetRGB(pixels[count], format, &r, &g, &b);
            hist[r]++;
        }
        count++;
    }

    //step2 for equalization : get the cumulative histogram

    int cumulhist[256] = { 0 };

    for (int i = 0; i < 256; i++)
    {
        if (i == 0)
            cumulhist[i] = hist[i];
        else
            cumulhist[i] = cumulhist[i-1] + hist[i];
    }

    //step3 for equalization : get the equalized histogram

    int equalizedhist[256] = { 0 };

    for (int i = 0; i < 256; i++)
    {
        equalizedhist[i] = (cumulhist[i] * 255) / length;
    }

    //step4 for equalization : get the equalized image

    count = 0;
    while (count < length)
    {
        int i = 0;
        int found = 1;
        while (i < factor && found)
        {
            uint upperbound = (i+1)*(255/factor);
            uint lowerbound = i*(255/factor);
            if (pixels[count] > lowerbound && pixels[count] < upperbound)
            {
                pixels[count] = SDL_MapRGB(format, equalizedhist[upperbound], equalizedhist[upperbound], equalizedhist[upperbound]);
                i++;
            }
            else
            {
                i++;
            }
        }
        count++;
    }

    SDL_UnlockSurface(surface); 
}