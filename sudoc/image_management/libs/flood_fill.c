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

int* get_coordinates(Uint32 *pixels, Uint32 color, int len)
{
    int first = 0;
    int last = 0;  

    int i = 0;
    while (i < len)
    {
        if (pixels[i] == color)
        {
            if (first == 0)
            {
                first = i;
                last = i;
                break;
            }
        }
        i++;
    }
    
    while (i < len)
    {
        if (pixels[i] == color)
        {
            last = i;
        }
        i++;
    }
    int* return_value = calloc(2,sizeof(int));
    return_value[0] = first;
    return_value[1] = last;

    return return_value;
}

void bringing_back(SDL_Surface *image)
{   
    Uint32* pixels = image->pixels;
    SDL_LockSurface(image);
    
    for (int i = 0; i < image->w * image->h; i++)
    {
        Uint8 r, g, b;
        SDL_GetRGB(pixels[i], image->format, &r, &g, &b);
        if (r != 255)
        {
            pixels[i] = SDL_MapRGB(image->format, 0, 0, 0);
        }
        if (b == 255)
        {
            pixels[i] = SDL_MapRGB(image->format, 255, 255, 255);
        }
    }


	SDL_UnlockSurface(image); 
}

void deleting(SDL_Surface* surface)
{
    SDL_LockSurface(surface);
    Uint32 *pixels = surface->pixels;
    int h = surface->h;
    int w = surface->w;
    //int length = h*w;

    for (int j = 0; j < h;j++)
    {
        for (int i = 0; i < w;i++)
        {
            Uint8 r, g, b;
            SDL_GetRGB(pixels[i + w * j], surface->format, &r, &g, &b);

            if (r == 255 && b == 0 && g == 0)
            {
                //red_encountered = 1;
                break;
            }
            else
            {
                pixels[i + j * w] = SDL_MapRGB(surface->format, 0, 0, 0);
            }
        }
    }

    for (int j = h -1; j >= 0; j--)
    {
        for (int i = w -1; i >= 0;i--)
        {
            Uint8 r, g, b;
            SDL_GetRGB(pixels[i + w * j], surface->format, &r, &g, &b);

            if (r == 255 && b == 0 && g == 0)
            {
                //red_encountered = 1;
                break;
            }
            else
            {
                pixels[i + j * w] = SDL_MapRGB(surface->format, 0, 0, 0);
            }
        }
    }



    SDL_UnlockSurface(surface);
}

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
    //int* coordinates = get_coordinates(pixels, finalcolor, length);
    //printf("First: %i, Last: %i\n", coordinates[0], coordinates[1]);
    
    //bring back the white pixels instead of the blue ones
    bringing_back(surface);
    deleting(surface);
    IMG_SavePNG(surface, "result.png");
    SDL_UnlockSurface(surface);
}
