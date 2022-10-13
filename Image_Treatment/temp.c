weight = (1/(2*pi*1.5)) * exp((-((pow(x,2) + pow(y*2))))/(2*pow(1.5,2)))






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
    Uint32 result[length]; 


    /*
    This is the basic kernel for implementation of 3*3
    
     float kernel[] = {1, 1, 1,
                    1, 1, 1, 
                    1, 1, 1};
    
    
    float kernel[] = {0.0625, 0.125, 0.0625,
                    0.125, 0.25, 0.125, 
                    0.0625, 0.125 , 0.0625};
    */
    float kernel[] = {0.0625, 0.125, 0.0625,
                    0.125, 0.25, 0.125, 
                    0.0625, 0.125 , 0.0625};
    //2ND STEP : ACCESSING EVERY PIXELS
    float res = 0;
    int i = 0;
    while (i < h)
    {
        int j = 0;
        while (j < w)
        {
            res = 0;
            printf("row = %i and column = %i \n", i, j);
            float number_of_box = 0;

            //accessing top left corner :
            int destination = (i-1)*w+(j-1);
            if (destination >= 0 && destination < length)  
            {
                res += pixels[destination] * kernel[0]; 

                number_of_box++; 
                printf("%d, %f", pixels[destination], res);
            }

            //accessing top corner :
            destination = (i-1)*w+j;
            if (destination >= 0 && destination < length)  
            {

                res += pixels[destination] * kernel[1]; 
                number_of_box++;
            }

            //accessing top right corner :
            destination = (i-1)*w+(j+1);
            if (destination >= 0 && destination < length)  
            {
                res += pixels[destination] * kernel[2]; 
                number_of_box++;
            }
           
            //accessing left :
            destination = i*w+(j-1);
            if (destination >= 0 && destination < length)  
            {
                res += pixels[destination] * kernel[3]; 
                number_of_box++;
            }
           
            //accessing center :
            destination = i*w+j;
            if (destination >= 0 && destination < length)  
            {
                res += pixels[destination] * kernel[4]; 
                number_of_box++;
            }
           
            //accessing right :
            destination = i*w+(j+1);
            if (destination >= 0 && destination < length)  
            {
                res += pixels[destination] * kernel[5]; 
                number_of_box++;
            }
           
            //accessing bottom left corner :
            destination = (i+1)*w+(j-1);
            if (destination >= 0 && destination < length)  
            {
                res += pixels[destination] * kernel[6]; 
                number_of_box++;
            }

            //accessing bottom corner :
            destination = (i+1)*w+j;
            if (destination >= 0 && destination < length)  
            {
                res += pixels[destination] * kernel[7]; 
                number_of_box++;
            }

            //accessing bottom right corner :
            destination = (i+1)*w+(j+1);
            if (destination >= 0 && destination < length)  
            {
                res += pixels[destination] * kernel[8]; 
                number_of_box++;
            }

            if (number_of_box != 0)
            {
                result[i*w+j] = res/(number_of_box); 
                //printf("res = %f, number_of_box = %f, result = %f\n", res, number_of_box, result[i*w+j]);
            }
            

            j++;
        }
        
        i++;
    }
    __Blurring_process(surface, result);
}
        