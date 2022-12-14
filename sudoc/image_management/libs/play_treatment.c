#include "../include/play_treatment.h"

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

void play_treatment(char* path)
{
    // - Create a surface from the colored image.
    SDL_Surface* surface = Load_image(path);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Convert the surface into grayscale.
    Surface_To_Grayscale(surface);
    //IMG_SavePNG(surface, "../web/website/src/components/Upload/process/1.png");

    // - Convert the surface into blurred surface
	Kernel_Convolution(surface);
    //IMG_SavePNG(surface, "../web/website/src/components/Upload/process/2.png");

    contrast(surface);
    IMG_SavePNG(surface, "./web/website/src/components/Upload/process/1.png");
    blackandwhite(surface);
    IMG_SavePNG(surface, "./web/website/src/components/Upload/process/2.png");
    // - Convert the surface into sobel surface 
	Kernel_Convolution_Sobel(surface);
    IMG_SavePNG(surface, "./web/website/src/components/Upload/process/3.png");

    SDL_Surface* surface2 = Load_image("./web/website/src/components/Upload/process/2.png");
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int* coordinates;
    coordinates = princip(surface);
    int ymin = coordinates[0] / surface->w;
    int ymax = coordinates[1] / surface->w;
    IMG_SavePNG(surface, "./web/website/src/components/Upload/process/4.png");
    
    Images *image = CV_SURFACE_TO_IMG(surface);
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
    if (angle != 0)
    {   
        SDL_Surface *rotated = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, 0, 0, 0, 0);
        SDL_FillRect(rotated, NULL, SDL_MapRGB(rotated->format, 0, 0, 0));
        rotate_image(surface, rotated, angle);
        image = CV_SURFACE_TO_IMG(rotated);
        if (image == NULL)
            errx(1, "Failed converting the surface to image");
        SDL_Surface *rotated2 = SDL_CreateRGBSurface(0, surface2->w, surface2->h, 32, 0, 0, 0, 0);
        SDL_FillRect(rotated2, NULL, SDL_MapRGB(rotated2->format, 0, 0, 0));
        rotate_image(surface2, rotated2, angle);
        surface2 = rotated2;
        //Hough Transform call
        lines = Hough_lines(image, 300, &n);
        merged = Hough_merge_lines(lines, n, 35, &n);
    }
    printf("Angle: %f",angle);
    Hough_draw_lines(image, image, merged, n, 2, CV_RGB(255, 0, 0));

    // - Save the image.
    surface = CV_IMG_TO_SURFACE(image);
    //IMG_SavePNG(surface, "lines.png");

    //Find intersection points
    int nbintersections = 0;
    int *intersections = Compute_intersections(merged, n,&nbintersections,surface->w,surface->h,ymin,ymax);
    int i = 0;
    while (i  < nbintersections)
    {
        Hough_draw_circle(image,image,intersections[2*i],intersections[2*i+1],5,5,CV_RGB(0, 0, 255));
        i++;
    }
    // - Save the image.
    surface = CV_IMG_TO_SURFACE(image);
    IMG_SavePNG(surface, "./web/website/src/components/Upload/process/5.png");
    
    //First free
    CV_FREE(&image);
    free(lines);
    free(merged);
    SDL_FreeSurface(surface);
    
    
    //Find grid
    int *grid = Get_grid(intersections, nbintersections);
    printf("Grid: %d %d %d %d",grid[0], grid[1], grid[2], grid[3]);

    // - Crop the image.
    int width = grid[2] - grid[0];
    int height = grid[3] - grid[1];

    int new_width = width/9;
    int new_height = height/9;
    printf("New width: %d New height: %d",new_width,new_height);
    char file[38];
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            SDL_Surface* croped = SDL_CreateRGBSurface(0,new_width,new_height,32,0,0,0,0);
            SDL_FillRect(croped, NULL, SDL_MapRGB(croped->format, 0, 0, 0));
            croped_image(surface2,croped, grid[0]+j*new_width+i, grid[1]+i*new_height+i,grid[0]+(j+1)*new_width+i,grid[1]+(i+1)*new_height+i );
            Clean_surface(croped);
            croped = resize(croped, 28, 28);
            sprintf(file, "./sudoc/neural_network/images/%d.png",9*i+j);
            //sprintf(file, "./build/%d.png",9*i+j);
            IMG_SavePNG(croped, file);
            SDL_FreeSurface(croped);
        }
    }

    // - Free the surface.

    
    free(intersections);
    free(grid);
    return;
}