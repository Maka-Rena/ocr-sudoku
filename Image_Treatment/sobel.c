#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

//Find the max in the list 
//used for resizing the datas
float find_max(float list[], int len)
{
     int max = 0;
     for(int i = 0; i < len; i++)
     {
		if (list[i] > max)
	    	max = list[i];
     }
     return max;
}


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
// renderer: Renderer to draw on.
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

//load the image that is already gray and blurred
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

void __sobel_transformation(SDL_Surface* surface, Uint32* resultgradient)//, Uint32* resultangle) 
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
        Uint32 color = resultgradient[count];
        //<= 50 works for non grayscaled images
        //<= 100 for grayscaled and blurred
        if (color <= 100)
            pixels[count] = SDL_MapRGB(format, 0, 0, 0);
        else

            pixels[count] = SDL_MapRGB(format, 255, 255, 255);
        count++;
    }
    SDL_UnlockSurface(surface);
}



void Kernel_Convolution(SDL_Surface* surface, int w, int h)
{
    /*
       Formula to acces values in array :
       i * WIDTH + j
       */

    /*
       Creating an array to store the result
       Creating the array for the pixel
       Getting the length 
       Getting the format
       */ 

    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int length = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_LockSurface(surface);

	//Create the lists to stock gradients 
    float *result_gradient;
    result_gradient = malloc(length * sizeof(Uint32));
    Uint32 *colored_gradient;
    colored_gradient = malloc(length * sizeof(Uint32));
    //Uint32 result_angle[w*h]; this variable is used to know the exact angle of an edge for every pixel

    uint kernelx[] = {-1, 0, 1,
        -2, 0, 2, 
        -1, 0, 1};

    uint kernely[] = {-1, -2, 1,
        0, 0, 0, 
        1, 2, 1};



    //2ND STEP : ACCESSING EVERY PIXELS
    int resx = 0;
    int resy = 0;
    int i = 0;
    while (i < h) 
    {
        int j = 0;
        while (j < w)
        {
            resx = 0;
            resy = 0;

            //FOR G(X):

            //accessing left :
            int destination = i*w+(j-1);
            if (destination >= 0 && destination < length)  
            {
                Uint8 r,g,b;
                SDL_GetRGB(pixels[destination], format, &r, &g, &b);

                resx = resx + r * kernelx[3]; 

            }


            //accessing right :
            destination = i*w+(j+1);
            if (destination >= 0 && destination < length)  
            {
                Uint8 r,g,b;
                SDL_GetRGB(pixels[destination], format, &r, &g, &b);

                resx = resx + r * kernelx[5]; 

            }


            //FOR G(Y):

            //accessing top:

            destination = (i-1)*w+j;
            if (destination >= 0 && destination < length)  
            {
                Uint8 r,g,b;
                SDL_GetRGB(pixels[destination], format, &r, &g, &b);

                resy = resy + r * kernely[1]; 

            }
            //accessing bottom :

            destination = (i+1)*w+j;
            if (destination >= 0 && destination < length)  
            {
                Uint8 r,g,b;
                SDL_GetRGB(pixels[destination], format, &r, &g, &b);

                resy = resy + r * kernely[7]; 

            }



            //FOR BOTH G(X) and G(Y):


            //accessing top left corner :
            destination = (i-1)*w+(j-1);
            if (destination >= 0 && destination < length)  
            {
                Uint8 r,g,b;
                SDL_GetRGB(pixels[destination], format, &r, &g, &b);

                resy = resy + r * kernely[0]; 
                resx = resx + r * kernelx[0]; 


            }

            //accessing top right corner :
            destination = (i-1)*w+(j+1);
            if (destination >= 0 && destination < length)  
            {
                Uint8 r,g,b;
                SDL_GetRGB(pixels[destination], format, &r, &g, &b);

                resy = resy + r * kernely[2]; 
                resx = resx + r * kernelx[2]; 

            }

            //accessing bottom left corner :
            destination = (i+1)*w+(j-1);
            if (destination >= 0 && destination < length)  
            {
                Uint8 r,g,b;
                SDL_GetRGB(pixels[destination], format, &r, &g, &b);

                resy = resy + r * kernely[6]; 
                resx = resx + r * kernelx[6]; 

            }

            //accessing bottom right corner :
            destination = (i+1)*w+(j+1);
            if (destination >= 0 && destination < length)  
            {
                Uint8 r,g,b;
                SDL_GetRGB(pixels[destination], format, &r, &g, &b);

                resy = resy + r * kernely[8]; 
                resx = resx + r * kernelx[8]; 

            }

            result_gradient[i*w+j] = sqrt((pow(resy,2))+pow(resx,2));
	    	
            //result_angle[i*w+j] = atan(resy/resx); 

            j++;

        }

        i++;
    }

	int max = find_max(result_gradient,w*h); //find the max

	//resizing all datas

	for(int i=0; i<w*h; i++)
	{
		colored_gradient[i] = (Uint32)((result_gradient[i]/max)*255);
        //angle_gradient[i] = (Uint32)(result_angle[i]/max)*255;
	}
    __sobel_transformation(surface, colored_gradient);//, angle_gradient);

    free(colored_gradient);
    free(result_gradient);
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

    //get the height of the texture
    int w, h;
    if (SDL_QueryTexture(texture, NULL, NULL, &w, &h) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    Kernel_Convolution(surface,w,h);

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
