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
		x++; 
    	j++;
	}
	SDL_UnlockSurface(croped); 
}

void Clean_surface(SDL_Surface *surface)
{
	Uint32* pixels = surface->pixels;
    int w = surface->w;
    int h = surface->h;

	SDL_LockSurface(surface);
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
			if (i>h/9 && i<8*h/9 && j>w/9 && j<8*w/9)
				continue;
            pixels[i*w + j] = SDL_MapRGB(surface->format, 0,0,0);;
        }
    }

	SDL_UnlockSurface(surface); 
}

