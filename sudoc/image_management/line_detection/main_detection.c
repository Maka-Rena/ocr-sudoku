#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./include/sobel.h"
#include "./include/hough_transform.h"



// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer:Renderer to draw on.
// colored: Texture that contains the colored image.
// texture_blurred: Texture that contains the blurred image.
void event_loop(SDL_Renderer* renderer, SDL_Texture* texture_sobel, SDL_Texture* texture_hough)
{    SDL_Event event;
    SDL_Texture* t = texture_sobel;

    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
                return;

            // If the window is resized, updates and redraws the diagonals.
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    draw(renderer, t);
                }
                break;
            case SDL_KEYDOWN :
				if (t == texture_sobel)
				{
                	draw(renderer, texture_hough);
                    t = texture_hough;
				}
                else
				{
					draw(renderer, texture_sobel);
                    t = texture_sobel;
				}
        }
    }
}

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
    
    // - Create a new texture from the surface.
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
		  errx(EXIT_FAILURE, "%s", SDL_GetError());
	
    //Pointer used in hough_transform
    double* resultangle;
    int length = surface->w * surface->h;
    resultangle = malloc(length * sizeof(double));

    //SOBEL

    Kernel_Convolution_Sobel(surface, resultangle);
    SDL_Texture* texture_sobel = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture_sobel == NULL)
		  errx(EXIT_FAILURE, "%s", SDL_GetError());

    //Hough
    
    hough_transform(surface, resultangle, renderer);
    SDL_Texture* texture_hough = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture_hough == NULL)
		  errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    // - Dispatch the events.
    event_loop(renderer,texture_sobel, texture_hough);



    //to save the blurred image in a png file
    IMG_SavePNG(surface, "final_picture.png");
    // - Destroy the objects.
	SDL_FreeSurface(surface);
    SDL_Quit();

    return EXIT_SUCCESS;
}   
