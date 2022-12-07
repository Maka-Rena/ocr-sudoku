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

    //int new_len = croped->w*croped->h;

	SDL_LockSurface(croped);
	int j = 0;
	int hmax = croped->h;
	while (hmax != 0)
	{
    	for (int i = 0; i < croped->w; i++)
    	{
			new_pixels[j] = pixels[begin + i]; 
    		j++;
    	}
		hmax--;
		begin = begin + image->w;
	}
	SDL_UnlockSurface(croped); 
}

