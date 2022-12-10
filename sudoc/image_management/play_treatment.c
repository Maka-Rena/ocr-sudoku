#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../libraries/matrix_lib/include/matrix.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./include/hough_transform.h"
#include "./include/black_n_white.h"
#include "./include/cell.h"
#include "./include/contrast_light.h"
#include "./include/crop_ia.h"
#include "./include/crop.h"
#include "./include/gaussian.h"
#include "./include/grayscale.h"
#include "./include/resize.h"
#include "./include/sobel.h"
#include "./include/to_matrix.h"

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
    if (argc == 1)
        errx(EXIT_FAILURE, "%s", "no arg");

    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a surface from the colored image.
    SDL_Surface* surface = Load_image(argv[1]);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Convert the surface into grayscale.
    Surface_To_Grayscale(surface);
    IMG_SavePNG(surface, "grayscale.png");

    // - Convert the surface into blurred surface
	Kernel_Convolution(surface);
    IMG_SavePNG(surface, "blurred.png");

    contrast(surface);
    IMG_SavePNG(surface, "contrast.png");
    blackandwhite(surface);
    IMG_SavePNG(surface, "black_n_white.png");
    
    // - Convert the surface into sobel surface 
	Kernel_Convolution_Sobel(surface);
    IMG_SavePNG(surface, "sobel.png");

    Image *image = CV_SURFACE_TO_IMG(surface);
    if (image == NULL)
        errx(1, "Failed converting the surface to image");

    //Hough Transform call
    int n = 0;
    int *lines = Hough_lines(image, 300, &n);
    int *merged = Hough_merge_lines(lines, n, 35, &n);
    float angle = Find_orientation(merged, n);
    printf("Angle: %f",angle);
    if (angle > 90 && angle < 135)
        angle = angle - 90;
    else if (angle > 135)
        angle = angle - 180;
    printf("Angle: %f",angle);
    Hough_draw_lines(image, image, merged, n, 2, CV_RGB(255, 0, 0));

    // - Save the image.
    surface = CV_IMG_TO_SURFACE(image);
    IMG_SavePNG(surface, "lines.png");

    //Find intersection points
    int nbintersections = 0;
    int *intersections = Find_intersections(merged, n,&nbintersections);
    int i = 0;
    while (i  < nbintersections*2)
    {
        Hough_draw_circle(image,image,intersections[2*i],intersections[2*i+1],5,5,CV_RGB(0, 0, 255));
        i++;
    }
    // - Save the image.
    surface = CV_IMG_TO_SURFACE(image);
    IMG_SavePNG(surface, "intersections.png");


    //Find cells
    /*int *cells = Get_grid(intersections, nbintersections);
    i = 0;
    surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    printf("%d %d %d %d\n",cells[0],cells[1],cells[2],cells[3]);
        SDL_Surface* croped = SDL_CreateRGBSurface(0, (cells[4*i+2]-cells[4*i])+1,(cells[4*i+3]-cells[4*i+1])+1,32,0,0,0,0);
        SDL_FillRect(croped, NULL, SDL_MapRGB(croped->format, 255, 255, 255));
        //Convert the pixel format to RGBA32
	    //croped_image(surface,croped, cells[4*i], cells[4*i+1], cells[4*i+2], cells[4*i+3]);
        char file[7];
        sprintf(file, "%d.png", i);
	    IMG_SavePNG(croped, file);
        SDL_FreeSurface(croped);*/
    // - Free the surface.
    SDL_FreeSurface(surface);
    CV_FREE(&image);

    free(lines);
    free(merged);
    return EXIT_SUCCESS;
}