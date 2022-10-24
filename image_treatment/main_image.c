#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./include/sobel.h"
#include "./include/gaussian.h"
#include "./include/grayscale.h"

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");
    
    // - Initialize the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a window.
    SDL_Window* window = SDL_CreateWindow("Plain Window", 0, 0, 640, 400,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a surface from the colored image.
    SDL_Surface* surface = Load_image(argv[1]);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Resize the window according to the size of the image.
    SDL_SetWindowSize(window, surface->w, surface->h);
    
    // - Create a texture from the colored surface.
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Convert the surface into grayscale.
    Surface_To_Grayscale(surface);

    // - Create a new texture from the grayscale surface.
    SDL_Texture* texture_gray = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture_gray == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    //to save the grayscale image in a png file
    IMG_SavePNG(surface, "final_picture.png");

    // - Free the surface.
    SDL_FreeSurface(surface);
    
    // - Dispatch the events.
    event_loop(renderer,texture, texture_gray);

    // - Destroy the objects.

	}
