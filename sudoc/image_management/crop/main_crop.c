#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./include/crop.h"

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* Load_image(const char* path)
{
    SDL_Surface* surface = IMG_Load(path);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface* res = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGB888,0);
    if (res == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(surface);
    return res;
}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 4)
        errx(EXIT_FAILURE, "Usage: image int1 int2");
    
    // - Initialize the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a surface from the colored image.
    SDL_Surface* surface = Load_image(argv[1]);
    if (surface == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	int begin = atoi(argv[2]);
	int end = atoi(argv[3]);
	//printf("%d",begin);
	//printf("%d",end);
	//SDL_Surface* croped = SDL_CreateRGBSurface(0,surface->w,surface->h,32,0,0,0,0);
	SDL_Surface* croped = SDL_CreateRGBSurface(0, (end - begin) % surface->w + 1 ,(end/surface->w) - (begin/surface->w) + 1,32,0,0,0,0);
	SDL_FillRect(croped, NULL, SDL_MapRGB(croped->format, 255, 255, 255));
	
	croped_image(surface,croped, begin, end);
	IMG_SavePNG(croped, "croped_picture.png");
	
    // - Destroy the objects.
	SDL_FreeSurface(surface);
	SDL_FreeSurface(croped);
    SDL_Quit();
	
    return EXIT_SUCCESS;
}   
