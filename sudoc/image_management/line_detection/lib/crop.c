#include "../include/crop.h"

void croped_image(SDL_Surface *image, SDL_Surface *croped, int begin, int end)
{   
    Uint32* pixels = image->pixels;
    Uint32* new_pixels = croped->pixels;
    if (pixels == NULL || new_pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int len = image->w*image->h;

	if (begin < 0 || end > len)
		errx(EXIT_FAILURE, "%s", "index out of borns");

    int new_len = rotated->w*rotated->h;

	SDL_LockSurface(croped);
	int j = 0;
    for (int i = begin; i < end; i++)
    {
		new_pixels[j] = pixels[i]; 
    	j++;
    }

	SDL_UnlockSurface(croped); 
}

