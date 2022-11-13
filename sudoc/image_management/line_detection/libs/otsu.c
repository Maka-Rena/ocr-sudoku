#include "../include/otsu.h"

float Otsu_Threshold(Uint32* pixels, int len, SDL_PixelFormat* format)
{
    int histogram[256] = {0};
    float len_f = (float) len;

    for (int i = 0; i < len; i++)
    {
        Uint8 r,g,b;
        SDL_GetRGB(pixels[i], format, &r, &g, &b);

        histogram[r] ++;
    }
    float sum = 0;
    for (int i = 0; i < 256; i++)
    {
        sum += i * histogram[i];
    }

    float sumB = 0;
    float q1 = 0;
    float q2 = 0;

    float var_max = 0;
    float threshold = 0;

    for (int j = 0; j < 256; j++)
    {
        q1 += (float) histogram[j];
        if (q1 == 0)
            continue;
        if (q1 == len)
            break;
        
        q2 = len_f - q1;

        sumB += j * histogram[j];

        float mu1 = sumB/q1;
        float mu2 = (sum - sumB)/q2;
        
        float sigma = q1 * q2 * ((mu1 - mu2) * (mu1 - mu2));

        if (sigma > var_max)
        {
            var_max = sigma;
            threshold = j;
        }
    }    

    return threshold / 255.0;
}



void otsu(SDL_Surface* surface)
{
    int w = surface->w; 
	int h = surface->h;
    int length = w * h;

    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_LockSurface(surface);

    int threshold = (int) Otsu_Threshold(pixels, length, format);

    //h and w are the height and width of the surface
    for (int h1 = 0; h1 < h; h++)
    {
        for (int w1 = 0; w1 < w; w++)
        {
            Uint8 r,g,b;
			SDL_GetRGB(pixels[w1*w+h], format, &r, &g, &b);

            if (r > threshold)
                pixels[w1*w+h] = SDL_MapRGB(format, 1, 1, 1);
            else
                pixels[w1*w+h] = SDL_MapRGB(format, 0, 0, 0);
        }
    }
    SDL_UnlockSurface(surface);
}