#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

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
void event_loop(SDL_Renderer* renderer, SDL_Texture* colored, SDL_Texture* texture_blurred)
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
                    draw(renderer, texture_blurred);
                    t = texture_blurred;
                }
                else
                {
                    draw(renderer, colored);
                    t = colored;
                }
                break;
        }
    }
}

//load the image that is already gray
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
    This is the basic kernel for implementation of 3*3
    
     float kernel[] = {1, 1, 1,
                    1, 1, 1, 
                    1, 1, 1};
    
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
		
						uint average = 0.3*r+ 0.59*g + 0.11*b;
                		res = res + average * kernel[0]; 
                		number_of_box++;
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
    
    // - Create a texture from the colored surface.
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    //Blur the picture
    Kernel_Convolution(surface);

    //Create texture from the blurred surface
    SDL_Texture* texture_blurred = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture_blurred == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    //to save the blurred image in a png file
    IMG_SavePNG(surface, "final_picture.png");

    // - Free the surface.
    SDL_FreeSurface(surface);


    // - Dispatch the events.
    event_loop(renderer, texture, texture_blurred);

    // - Destroy the objects.    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture_blurred);
    SDL_Quit();

    return EXIT_SUCCESS;
    
}
