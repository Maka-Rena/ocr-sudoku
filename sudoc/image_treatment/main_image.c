#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./include/sobel.h"
#include "./include/gaussian.h"
#include "./include/grayscale.h"

#include "./include/black_n_white.h"
#include "./include/contrast_light.h"


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
void event_loop(SDL_Renderer* renderer, SDL_Texture* colored, SDL_Texture* texture_gray, SDL_Texture* texture_blurred, SDL_Texture* texture_sobel, SDL_Texture* blackandwhite, SDL_Texture* contrasted_texture)
{
    SDL_Event event;
    SDL_Texture* t = colored;

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
				if (t == colored)
				{
                	draw(renderer, texture_gray);
                    t = texture_gray;
				}
				else if (t == texture_gray)
				{
					draw(renderer, contrasted_texture);
                    t = texture_blurred;
				}
                else if (t == contrasted_texture)
				{
					draw(renderer, texture_blurred);
                    t = texture_blurred;
				}
				else if (t == texture_blurred)
				{
					draw(renderer, blackandwhite);
                    t = blackandwhite;
				}
                else if (t == blackandwhite)
                {
                    draw(renderer, texture_sobel);
                    t = texture_sobel;
                }
				else
				{
					draw(renderer, colored);
                    t = colored;
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

    // - Convert the surface into grayscale.
    Surface_To_Grayscale(surface);

    // - Create a new texture from the grayscale surface.
    SDL_Texture* texture_gray = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture_gray == NULL)
		  errx(EXIT_FAILURE, "%s", SDL_GetError());

    contrast(surface);

    SDL_Texture* contrasted_texture = SDL_CreateTextureFromSurface(renderer, surface);

	  // - Convert the surface into blurred surface
	  Kernel_Convolution(surface);

    // - Create a new texture from the blurred surface.
    SDL_Texture* texture_blurred = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture_blurred == NULL)
		  errx(EXIT_FAILURE, "%s", SDL_GetError());

    blackandwhite(surface);

    SDL_Texture* blackandwhite = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture_blurred == NULL)
		  errx(EXIT_FAILURE, "%s", SDL_GetError());


	  // - Convert the surface into sobel surface 
	  Kernel_Convolution_Sobel(surface);

	  // - Create a new texture from the sobel surface.
    SDL_Texture* texture_sobel = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture_sobel == NULL)
		  errx(EXIT_FAILURE, "%s", SDL_GetError());

    //to save the blurred image in a png file
    IMG_SavePNG(surface, "final_picture.png");

    // - Free the surface.
    SDL_FreeSurface(surface);
    
    // - Dispatch the events.
    event_loop(renderer,texture,texture_gray, texture_blurred,texture_sobel, blackandwhite, contrasted_texture);

    // - Destroy the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture_gray);
    SDL_DestroyTexture(texture_blurred);
    SDL_DestroyTexture(texture_sobel);
    SDL_DestroyTexture(blackandwhite);
    SDL_DestroyTexture(contrasted_texture);
    SDL_Quit();

    return EXIT_SUCCESS;
}   
