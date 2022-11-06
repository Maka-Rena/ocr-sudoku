#include "../include/black_n_white.h"

double pw2(double x)
{
    return x * x;
}
int threshold(Uint32* pixels, int length, SDL_PixelFormat* format)
{
    int count = 0;
    Uint32 *h;
    h = calloc(256, sizeof(Uint32));
    while (count < length)
    {
        Uint8 r,g,b;
        SDL_GetRGB(pixels[count], format, &r, &g, &b);
        h[r] += 1;
        count++;
    }

    double prob[256];
    size_t min_i = 256;
    double min_var = DBL_MAX;

     for (size_t i = 0; i < 256; ++i)
        prob[i] = (double)h[i] / (double)length;

    for (size_t j = 1; j < 255; ++j) {
        double m_under = 0, c = 0;
        for (size_t k = 0; k < j; ++k) {
            m_under += k * prob[k];
            c += prob[k];
        }

        // next iteration if a class is empty
        if (c == 0 || c == 1)
            continue;
        m_under /= c;

        double m_up = 0;
        for (size_t k = j; k < 256; ++k)
            m_up += k * prob[k];
        m_up /= (1 - c);

        double var = 0;
        for (size_t k = 0; k < j; ++k)
            var += pw2(k - m_under) * prob[k];
        for (size_t k = j; k < 256; ++k)
            var += pw2(k - m_up) * prob[k];

        if (min_var > var) {
            min_i = j;
            min_var = var;
        }
    }
    free(h);
    return min_i;

}




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

    int th = threshold(pixels, length, format);
    int count = 0;
    while (count < length)
    {
        Uint8 r,g,b;
		SDL_GetRGB(pixels[count], format, &r, &g, &b);

        if (((r+b+g)/3) >= th)
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