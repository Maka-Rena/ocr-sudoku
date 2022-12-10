#include "../include/crop.h"

void croped_image(SDL_Surface *image, SDL_Surface *croped, int x1,int y1,int x2, int y2)
{   
    Uint32* pixels = image->pixels;
    Uint32* new_pixels = croped->pixels;
    int len = croped->w*croped->h;

	SDL_LockSurface(croped);
	int j = 0;
	int x = x1;
	int y = y1;
	while (y != y2 && j < len)
	{
		if (x == x2)
		{
			x = x1;
			y++;
		}
		new_pixels[j] = pixels[image->w*y + x]; 
    	j++;
	}
	SDL_UnlockSurface(croped); 
}

