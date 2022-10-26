#include "../include/gaussian.h"

//draw the blurred surface from the kernel convulution
void __Blurring_process(SDL_Surface* surface, Uint32* result)
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
    while (count < len)
    {
        uint color = result[count];
        pixels[count] = SDL_MapRGB(format, color, color, color);
        count++;
    }
    SDL_UnlockSurface(surface);
}

/*function to find the new surface to draw :
    Traverse every pixel of the surface
    For the 8 neighbors and the pixel :
    - The function will take each of the neighbors 
        do a multiplication with the kernel placed at the same position
        add every multiplication
        Divide by the sum boxes encountered (usually 9 except in the borders)
    It will then called the function to do the drawing of the new surface 
*/
void Kernel_Convolution(SDL_Surface* surface)
{
	//Stocking the width, height, len and array for the results

	int w = surface->w;
	int h = surface->h;
    int length = surface->w * surface->h;
    Uint32 *result;
    result = malloc(length * sizeof(Uint32));

    /*
        Formula to acces values in array :
        i * WIDTH + j
    */

    /*
        Creating the array for the pixel
        Getting the length 
        Getting the format
    */ 
    
    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_LockSurface(surface); 

    /*
    This is the basic kernel for implementation of 3
    */
    /*uint kernel[] = {1, 2, 1,
                    2, 4, 2, 
                    1, 2 , 1};
    */
    uint kernel[] = {1, 1, 1,
                    1, 1, 1, 
                    1, 1, 1};
    //2ND STEP : ACCESSING EVERY PIXELS
    uint res = 0;
    int i = 0;
    while (i < h) 
    {
        int j = 0;
        while (j < w)
        {
            res = 0;
            uint number_of_box = 0;
			uint k = 0;
			//testing all pixels around
			for (int y = -1; y<2;y++)
			{
				for (int x = -1; x<2;x++)
				{
            		int destination = (i+y)*w+(j+x);
            		if (destination >= 0 && destination < length)  
            		{
						Uint8 r,g,b;
						SDL_GetRGB(pixels[destination], format, &r, &g, &b);
		
                		res = res + r * kernel[k]; 
                		number_of_box++;
						k++;
					}
				}
			}

            if (number_of_box != 0)
            {
				uint newcolor = res / number_of_box;
            	result[i*w+j] = SDL_MapRGB(format, newcolor, newcolor, newcolor); 
            	//printf("res = %f, number_of_box = %f, result = %f\n", res, number_of_box, result[i*w+j]);
			}
            j++;
        }
        
        i++;
    }
    __Blurring_process(surface, result);
    free(result);
}
