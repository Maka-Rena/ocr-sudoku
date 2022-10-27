#include "../include/contrast_light.h"


void contrast(SDL_Surface* surface)
{
    int length = surface->w * surface->h;
    int factor = 10;
 
    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_LockSurface(surface); 
    uint max = pixels[0];
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
                if (upperbound > max)
                    max = upperbound;
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


//FOR LIGHTNING PROBLEM V1
    /*count = 0;
    while (count < length)
    {
        Uint8 r, g, b;
        SDL_GetRGB(pixels[count], format, &r, &g, &b);
        uint color = 255 - (r)*(255/max);
        printf("%u\n", color);
        pixels[count] = 255 - (r)*(255/max);//SDL_MapRGB(format, color, color, color);
        count++;
    }*/


//FOR LIGHTNING PROBLEM V2
/*
    //step2 for equalization : calculate probabilty of each pixel intensity
    float prob[256] = {0};
    for (int i = 0; i < 256;i++)
    {
        prob[i] =  ((float) hist[i])/length;
    }

    //step3 : cumulative probability
    float cumulativ[256] = {0};
    cumulativ[0] = prob[0];
    for (int i = 1; i < 256;i++)
    {
        cumulativ[i] =  ((prob[i] + cumulativ[i-1])* 255) / length;
    }

    count = 0;
    while (count < 256)
    {
        uint new_color = cumulativ[count];
        pixels[count] = SDL_MapRGB(format, new_color, new_color, new_color);
        count++;
    }*/
}