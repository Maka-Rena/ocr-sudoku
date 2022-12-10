#include "../include/flood_fill.h"

void flood_fill(Uint32 *pixels, SDL_PixelFormat *format,
                Uint32 color, int i, int j, int w, int h, int* number_of_pixels)
{
    if (i < 0 || i >= w || j < 0 || j >= h)
    {
        return;
    }
    else
    {
        Uint8 r, g, b;
        SDL_GetRGB(pixels[i + j * w], format, &r, &g, &b);
        if (r == 255)
        {
            pixels[i + j * w] = color;
            (*number_of_pixels += 1);
           // printf("%i\n", *number_of_pixels);
            flood_fill(pixels, format, color, i + 1, j, w, h, number_of_pixels);
            flood_fill(pixels, format, color, i - 1, j, w, h, number_of_pixels);
            flood_fill(pixels, format, color, i, j + 1, w, h, number_of_pixels);
            flood_fill(pixels, format, color, i, j - 1, w, h, number_of_pixels);
        }

    
    }
    
}


void flood_fill_final(Uint32 *pixels, SDL_PixelFormat *format,
                Uint32 color, int i, int j, int w, int h)
{
    if (i < 0 || i >= w || j < 0 || j >= h)
    {
        return;
    }
    else
    {
        Uint8 r, g, b;
        SDL_GetRGB(pixels[i + j * w], format, &r, &g, &b);
        if (b == 255)
        {
            pixels[i + j * w] = color;
            flood_fill_final(pixels, format, color, i + 1, j, w, h);
            flood_fill_final(pixels, format, color, i - 1, j, w, h);
            flood_fill_final(pixels, format, color, i, j + 1, w, h);
            flood_fill_final(pixels, format, color, i, j - 1, w, h);
        }

    
    }
    
}

//void get_coordinates(Uint32 *pixels, )


void princip(SDL_Surface* surface)
{
    SDL_LockSurface(surface);
    Uint32 *pixels = surface->pixels;
    int h = surface->h;
    int w = surface->w;
    int length = h*w;
    int* number_of_pixels = calloc(1,sizeof(int));
    int max = 0;
    int coordinate = 0;

    SDL_PixelFormat *format = surface->format;

    Uint32 color = SDL_MapRGB(format, 0, 0, 255);

    for (int i = 0; i < length;i++)
    {
        Uint8 r, g, b;
        SDL_GetRGB(pixels[i], format, &r, &g, &b);
        if (r == 255)
        {
            *number_of_pixels = 0; 
            flood_fill(pixels, format, color, i%w, i/w, w, h, number_of_pixels);
            if (*number_of_pixels > max)
            {
                max = *number_of_pixels;
                coordinate = i;
            }

        }
    }  
    Uint32 finalcolor = SDL_MapRGB(format, 255, 0, 0);
    flood_fill_final(pixels, format, finalcolor, coordinate%w, coordinate/w, w, h); 
    SDL_UnlockSurface(surface);
}
