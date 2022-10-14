#include "../include/rotate.h"

void rotate_image(SDL_Surface *image, SDL_Surface *rotated, float teta)
{
    Uint8 r, g, b;
    
    Uint32* pixels = image->pixels;
    Uint32* new_pixels = rotated->pixels;
    if (pixels == NULL || new_pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int w = image->w;
    int h = image->h;
    int new_w = rotated->w;
    int new_h = rotated->h;

    float angle = teta * M_PI / 180;
    float cos_angle = cos(angle);
    float sin_angle = sin(angle);

    for (int i = 0; i < new_w; i++)
    {
        for (int j = 0; j < new_h; j++)
        {
            int x = (i - new_w / 2) * cos_angle - (j - new_h / 2) * sin_angle + w / 2;
            int y = (i - new_w / 2) * sin_angle + (j - new_h / 2) * cos_angle + h / 2;
            if (x >= 0 && x < w && y >= 0 && y < h)
            {
                SDL_GetRGB(pixels[y * w + x], image->format, &r, &g, &b);
                new_pixels[j * new_w + i] = SDL_MapRGB(rotated->format, r, g, b);
            }
        }
    }

    SDL_FreeSurface(image);
    SDL_Quit();
}