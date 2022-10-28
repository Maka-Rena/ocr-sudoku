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
   /* Uint32 *res;
    res = malloc(length * sizeof(Uint32));

    int w = surface->w;
    int h = surface->h;
    int whitepixel;
    count = 0;
    while (count < h) 
    {
        int j = 0;
        while (j < w)
        {
            whitepixel = 0;

            double number_of_box = 0;
            int k = 0;
            //testing all pixels around
            for (int y = -1; y<2;y++)
            {
                for (int x = -1; x<2;x++)
                {
                    int destination = (count+y)*w+(j+x);
                    if (destination >= 0 && destination < length)  
                    {
                        Uint8 r,g,b;
                        SDL_GetRGB(pixels[destination], format, &r, &g, &b);
                        if (r == 0)
                        {
                            whitepixel += 1;
                        }

                        k++;
                        if (x != 0 && y != 0)
                            number_of_box ++;
                        else if ( x == 0 && y == 0)
                            number_of_box += 4;
                        else 
                            number_of_box += 2;
                    }
                }
            }
            if (whitepixel >= 4)
            {
                res[count] = (uint) 255;
            }
            else
            {
                res[count] = (uint) 0;
            }
            j++;
        }
        
        count++;
    }

    count = 0;
    while (count < length)
    {
        uint color = res[count];
        pixels[count] = SDL_MapRGB(format, color, color, color);
        count++;
    }*/
}