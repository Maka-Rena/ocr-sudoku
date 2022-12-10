#include "../include/crop_ia.h"

void crop_9(SDL_Surface* surface, int* coordinates)
{
    int length = sizeof(coordinates) / sizeof(int*);

    for (int i = 0; i < length-3; i += 2)
    {
        SDL_Surface* cropped = SDL_CreateRGBSurface(0, surface->h, surface->w, 32, 0, 0, 0, 0);
        croped_image(surface, cropped, coordinates[i], coordinates[i+1], coordinates[i+2], coordinates[i+3]);
        SDL_Surface* resized = resize(cropped, 28, 28);
        char* name = malloc(sizeof(char) * 5);
        name = i + ".png";
        IMG_SavePNG(resized, name);
        free(resized);
        free(cropped);
    }
}