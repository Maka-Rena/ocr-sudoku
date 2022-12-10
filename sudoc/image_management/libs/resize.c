#include "../include/resize.h"

/*
SDL_Surface* Resize(SDL_Surface* not_resized)
{
    //SDL_Surface* rotozoomSurface(SDL_Surface* image, double angle, double zoom, double smooth);
    return rotozoomSurface(not_resized, 0, >2, 1);
}*/
double bilinearly_interpolate(unsigned int top, unsigned int bottom,
                              unsigned int left, unsigned int right,
                              double horizontal_position,
                              double vertical_position, Uint32* pixels, int width, SDL_PixelFormat* format)
{
    // Determine the values of the corners.
     Uint8 r,g,b;
    SDL_GetRGB(pixels[left * width + top], format, &r, &g, &b);
    double top_left = r;

    SDL_GetRGB(pixels[right * width + top], format, &r, &g, &b);
     double top_right = r;

    SDL_GetRGB(pixels[left * width + bottom], format, &r, &g, &b);
    double bottom_left = r;

    SDL_GetRGB(pixels[right * width + bottom], format, &r, &g, &b);
    double bottom_right = r;


    // Figure out "how far" the output pixel being considered is
    // between *_left and *_right.
    double horizontal_progress = horizontal_position - (double)left;
    double vertical_progress = vertical_position - (double)top;

    // Combine top_left and top_right into one large, horizontal
    // block.
    double top_block = top_left + horizontal_progress * (top_right - top_left);

    // Combine bottom_left and bottom_right into one large, horizontal
    // block.
    double bottom_block =
        bottom_left + horizontal_progress * (bottom_right - bottom_left);

    // Combine the top_block and bottom_block using vertical
    // interpolation and return as the resulting pixel.
    return top_block + vertical_progress * (bottom_block - top_block);
}

SDL_Surface* resize(SDL_Surface* surface, unsigned int newwidth, unsigned int newheight)
{
    
    SDL_PixelFormat* format = surface->format;
    SDL_Surface* resized = SDL_CreateRGBSurface(0, newheight, newwidth, 32,0,0,0,0); 
    Uint32* pixels_surface = surface->pixels;
    Uint32* pix_resized = resized->pixels;
    unsigned int width = surface->w;
    unsigned int height = surface->h;

    double xscale = newwidth / (double)width;
    double yscale = newheight / (double)height;

    for (unsigned int x = 0; x < newwidth; x++)
    {
        for (unsigned int y = 0; y < newheight; y++)
        {
            double oldx = x / xscale;
            double oldy = y / yscale;

            //floor function returns the largest integer that is smaller than or equal to x
            unsigned int top = floor(oldy);
            unsigned int bottom = top + 1;
            unsigned int left = floor(oldx);
            unsigned int right = left + 1;

            if (top < height && left < width && bottom < height
                && right < width)
            {
                double pixel = bilinearly_interpolate(
                    top, bottom, left, right, oldx, oldy, pixels_surface, width, format);
                
                pix_resized[x*28+y] = SDL_MapRGB(format, pixel, pixel, pixel); 
            }
        }
    }

    return resized;
}
