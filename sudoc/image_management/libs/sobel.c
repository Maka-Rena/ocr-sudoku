#include "../include/sobel.h"

//Find the max in the list 
//used for resizing the datas
double find_max(double list[], int len)
{
     int max = 0;
     for(int i = 0; i < len; i++)
     {
		if (list[i] > max)
	    	max = list[i];
     }
     return max;
}

int threshold_sob(Uint32* pixels, int length, SDL_PixelFormat* format)
{
    //Fing the average britness of the image
    int count = 0;
    Uint32* h;
    h = calloc(256, sizeof(Uint32));
    while (count < length)
    {
        Uint8 r,g,b;
        SDL_GetRGB(pixels[count], format, &r, &g, &b);
        h[r] += 1;
        count++;
    }

    double prob[256];
    size_t min_i = 256;
    double min_var = DBL_MAX;

     for (size_t i = 0; i < 256; ++i)
        prob[i] = (double)h[i] / (double)length;

    for (size_t j = 1; j < 255; ++j) {
        double m_under = 0, c = 0;
        for (size_t k = 0; k < j; ++k) {
            m_under += k * prob[k];
            c += prob[k];
        }

        // next iteration if a class is empty
        if (c == 0 || c == 1)
            continue;
        m_under /= c;

        double m_up = 0;
        for (size_t k = j; k < 256; ++k)
            m_up += k * prob[k];
        m_up /= (1 - c);

        double var = 0;
        for (size_t k = 0; k < j; ++k)
            var += pow(k-m_under, 2) * prob[k];
        for (size_t k = j; k < 256; ++k)
            var += pow(k-m_up, 2) * prob[k];

        if (min_var > var) {
            min_i = j;
            min_var = var;
        }
    }
    free(h);
    return min_i;
}

void __sobel_transformation(SDL_Surface* surface, double* resultgradient)//, Uint32* resultangle) 
{
    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    //int th = threshold_sob(pixels, len, format);

    SDL_LockSurface(surface);
    int count = 0;
    while (count < len)
    {
        double color = resultgradient[count];
        //<= 50 works for non grayscaled images
        //<= 100 for grayscaled and blurred
        if (color > 80)
            pixels[count] = SDL_MapRGB(format, 255, 255, 255);
        else
            pixels[count] = SDL_MapRGB(format, 0, 0, 0);
        count++;
    }
    SDL_UnlockSurface(surface);
}


void Kernel_Convolution_Sobel(SDL_Surface* surface)
{
	int w = surface->w; 
	int h = surface->h;
    int length = surface->w * surface->h;
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

    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_LockSurface(surface);

	//Create the lists to stock gradients 
    double *result_gradient;
    result_gradient = malloc(length * sizeof(double));
    double *colored_gradient;
    colored_gradient = malloc(length * sizeof(double));
    //Uint32 result_angle[w*h]; this variable is used to know the exact angle of an edge for every pixel

    double kernelx[] = {-1, 0, 1,
        -2, 0, 2, 
        -1, 0, 1};

    double kernely[] = {-1, -2, -1,
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

            result_gradient[i*w+j] = abs(resy) + abs(resx);

			//result_angle[i*w+j] = atan(resy/resx); 

            j++;
        }
        i++;
    }

	int max = find_max(result_gradient,w*h); //find the max

	//resizing all datas
	for(int i=0; i<w*h; i++)
	{
		colored_gradient[i] = ((result_gradient[i]/max)*255);
        //angle_gradient[i] = (Uint32)(result_angle[i]/max)*255;
	}

    __sobel_transformation(surface, colored_gradient);//, angle_gradient);

    free(colored_gradient);
    free(result_gradient);
}

