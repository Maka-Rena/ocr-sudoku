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
    int width = surface->w;
    int height = surface->h;
    int length = width * height;

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

                /*Uint8 r,g,b;
                SDL_GetRGB(pixels_surface[pixel], format, &r, &g, &b);*/
                
                pix_resized[x*28+y] = SDL_MapRGB(format, pixel, pixel, pixel); 
                //updatePixelToSameValue(&(newimage.pixels[x][y]), pixel);
            }
        }
    }

    return resized;
}


/*
SDL_Surface* Resize(SDL_Surface* surface)
{
        Formula to acces values in array :
        i * WIDTH + j
    

    //creates a 28 by 28 image
    SDL_Surface* resized = SDL_CreateRGBSurface(0, 28, 28, 32,0,0,0,0); 

    //if scale < 1 then we round down else we round up

    float scale_x = 28/surface->w; //width of the new surface / width of the surface
    float scale_y = 28 / surface->h; //height of the new surface /height of the surface


    Uint32* pixels_surface = surface->pixels;
    Uint32* pix_resized = resized->pixels;
    SDL_PixelFormat* format = surface->format;

    int length_resized = 784; //28*28
    int len_surf = surface->w * surface->h;
    int count = 0;
    int countinsurface = 0;
    int ratio = len_surf / 784;

    while (countinsurface < len_surf && count < 784)
    {

        int sum = 0;
        int count2 = 0;
        while (countinsurface < len_surf && count2 < ratio)
        {
            Uint8 r,g,b;
            SDL_GetRGB(pixels_surface[countinsurface], format, &r, &g, &b);
            sum += r;
            countinsurface++;
            count2++;
        }
        double newcolor = sum/ratio;
        pix_resized[count] = SDL_MapRGB(format, newcolor, newcolor, newcolor); 

        count++;


    }
    return resized;


}

SDL_Surface* Resize(SDL_Surface* surface)
{
    //creates a 28 by 28 image
    SDL_Surface* resized = SDL_CreateRGBSurface(0, 28, 28, 32, 255, 255, 255, 255); 

    Uint32* pixels_surface = surface->pixels;
    Uint32* pix_resized = resized->pixels;
    SDL_PixelFormat* format = surface->format;

    int dx = 0;
    while (dx < 28)
    {
        int dy = 0;
        while (dy < 28)
        {
            Uint8 r,g,b;
            SDL_GetRGB(pixels_surface[dx*(surface->w/28)*28 + dy*(surface->h/28)], format, &r,&g,&b); 
            pix_resized[dx*28 + dy] = SDL_MapRGB(format, r, r, r);
            dy++;
        }
        dx++;
    }
    return resized;
}

//dest[dx,dy] = src[dx*src_width/dest_width,   dy*src_height/dest_height]

{


    

    int i = 0;
    while (i < h) 
    {
        int j = 0;
        while (j < w)
        {
            res = 0;
            double number_of_box = 0;
			int k = 0;
			//testing all pixels around
			for (int y = -1; y<2;y++)
			{
				for (int x = -1; x<2;x++)
				{
            		int destination = (i+y)*w+(j+x);
            		if (destination >= 0 && destination < length)  
            		{
						



                        if (x != 0 && y != 0)
							number_of_box ++;
						else if ( x == 0 && y == 0)
							number_of_box += 4;
						else 
							number_of_box += 2;
					}
				}
			}
            j++;
        }
        
        i++;
    }
}*/