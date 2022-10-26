#include "../include/grayscale.h"

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
void event_loop(SDL_Renderer* renderer, SDL_Texture* colored, SDL_Texture* texture_gray, SDL_Texture* texture_blurred, SDL_Texture* texture_sobel, SDL_Texture* blackandwhite)
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

// Converts a colored pixel into grayscale.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
void __Pixel_To_Grayscale(Uint32 pixel_color, SDL_PixelFormat* format, Uint32 *color, uint *average)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    
    *average = 0.3*r + 0.59*g +0.11*b;
    *color = SDL_MapRGB(format, *average, *average, *average);
}

void Surface_To_Grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    int len = surface->w * surface->h;
    
    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_LockSurface(surface);
    int count = 0;

    //histogram to count the number of time a pixel color appears
    int hist[256] = { 0 };
    
    
    while (count < len)
    {
        Uint32 color;
        uint average;
        __Pixel_To_Grayscale(pixels[count], format, &color, &average);
        
        //step1 for equalization : get the histogram 
        hist[average]++;
        
        pixels[count] = color;
        count++;
    }

   /* //step2 for equalization : calculate probabilty of each pixel intensity
    float prob[256] = {0};
    for (int i = 0; i < 256;i++)
    {
        prob[i] =  ((float) hist[i])/len;
    }

    //step3 : cumulative probability
    float cumulativ[256] = {0};
    cumulativ[0] = prob[0];
    for (int i = 1; i < 256;i++)
    {
        cumulativ[i] =  ((prob[i] + cumulativ[i-1])* 255) / len;
    }

    count = 0;
    while (count < 256)
    {
        uint new_color = cumulativ[count];
        pixels[count] = SDL_MapRGB(format, new_color, new_color, new_color);
        count++;
    }*/
    SDL_UnlockSurface(surface);
}


