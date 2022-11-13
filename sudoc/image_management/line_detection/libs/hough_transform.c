#include "../include/hough_transform.h"


/*

The thing doesn't work probably because of the draw function
Try to make an array point with x1,y1,x2,y2 values in the main function
and update it here, then draw the points in the main 
*/


double degrees_ToRadians(double degrees)
{
    return degrees * M_PI / 180.0;
}

double radian_To_Degree(double radian)
{
    return radian * 180.0 / M_PI;
}

void draw_lines(SDL_Surface* surface, int x1, int y1, int x2, int y2, SDL_Renderer* renderer)
{
    Uint32* pixels = surface->pixels;
    int w = surface->w;
    int h = surface ->h;
    int len = w*h;
    SDL_PixelFormat* format = surface->format;
    int dx=abs(x2-x1);
    int dy=abs(y2-y1);
 
    int step;
    if(dx>=dy)
        step=dx;
    else
        step=dy;
 
    dx=dx/step;
    dy=dy/step;
 
    int x = x1;
    int y = y1;
 
    int i=1;
    while(i<=step)
    {
        int destination = x*w+y;
        if (destination < len && destination >0)
            pixels[destination] = SDL_MapRGB(format, 255, 0, 0);
        x=x+dx;
        y=y+dy;
        i=i+1;
    }
    //We draw on the render, but outcome is black probably because we draw a picture on it afterwards
}

void hough_transform(SDL_Surface* surface, double* resultangle, SDL_Renderer* renderer)
{
    float threshold = 0.4;

    Uint32* pixels = surface->pixels;
    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    int w = surface->w;
    int h = surface->h;
    int len = w * h;

    const int diagonal = sqrt(w * w + h * h); //images' diagonal line

    

    double *accumulation;
    accumulation = calloc(len, sizeof(double)); //sets every adress to zero

    int p; //rho 
    double max = 0;
    int x = 0;
    int y = 0;

    while (y < h)
    {
        while (x < w)
        {
            int i = y*w + x; 
            Uint8 r,g,b;
            SDL_GetRGB(pixels[i], format, &r, &g, &b);
        
            if (r == 0) //the pixel is black therefore cannot be an edge
            {
                x++; //we go to the next pixel
            }
            else
            {
                int theta = 0;
                while (theta <= 180)
                {
                    p = x * cos(theta) + y * sin(theta);
                    /*where ρ is the length of the normal line
                    θ is the angle between the normal line and the x axis.*/

                    int destination = i;
                    accumulation[destination] = accumulation[destination] + 1;
                    theta += 1;
                    if (accumulation[destination] > max)
                    {
                        max = accumulation[destination + diagonal];
                    }
                    theta += 1;
                }
                x++;
            }
        }
        y++;
    }


    //int lineThreshold = max * THRESHOLD;

    //these values are here only for a better understanding
    int accu_h = h;
    int accu_w = w;

    int j = 0;

    for (int i = 0;i< len;i++)
    {
        pixels[i] = 0;
    }
    while (j < accu_h)
    {
        int i = 0;
        while (i < accu_w)
        {
            if (accumulation[j*w + i] > threshold)
            {
                int max = accumulation[j*w + i]; 
                for(int y = -4; y <= 4;y++)
                {
                    for (int x = -4; x < 4 ;x++)
                    {
                        if ((y + j >= 0 && y+j < w) && (x+i >= 0 && x+i < w))
                        {
                            if (accumulation[j*w + i] > max)
                            {
                                max = accumulation[j*w + i];
                                x = 5;
                                y = 5;
                            }
                        }
                    }
                }
                if(max > accumulation[j*w + i])  
                    continue;  

                int x1,y1,x2,y2;
                x1 = y1 = x2 = y2 = 0; 

                if (i >= 45 && i <= 135)
                {
                    //y = (r - x cos(t)) / sin(t)

                    x1 = 0;  
                    y1 = ((double)(j-(h/2)) - ((x1 - (h/2) ) * cos(i))) / sin(i) + (h / 2);  
                    x2 = w - 0;  
                    y2 = ((double)(j-(h/2)) - ((x2 - (w/2) ) * cos(i))) / sin(i) + (h / 2); 
                    draw_lines(surface,x1,y1,x2,y2, renderer);
                }
            }
            i++;
        }
        j++;
    }

    //converting in carthesian

   // x0 = p*cos(theta) & y0 = p*sin(θ)

    
}