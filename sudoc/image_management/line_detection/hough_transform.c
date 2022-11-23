#include <err.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
//#include <math.h>
#define M_PI 3.14159265358979323846

struct line
{
    int x1;
    int y1;
    int x2;
    int y2;
    double angle;
    int flag;
};


double cosine(double x)
{
    return 1 - (x*x)/(2) + (x*x*x*x)/24 - (x*x*x*x*x*x)/720 + (x*x*x*x*x*x*x*x)/40320;
}

double sinus(double x)
{
    return x - (x*x*x)/6 + (x*x*x*x*x)/120 - (x*x*x*x*x*x*x)/5040 + (x*x*x*x*x*x*x*x*x)/363880;
}

double atang2(double x)
{
    return x - (1/3)*(x*x*x) + (1/5)*(x*x*x*x*x) - (1/7)*(x*x*x*x*x*x*x) + (1/9)*(x*x*x*x*x*x*x*x*x);
}

double minim(double a, double b)
{
    return a > b ? b : a;
}

double maxim(double a, double b)
{
    return a < b ? b : a;
}

double root(double n)
{
    // Max and min are used to take into account numbers less than 1
    double lo = minim(1, n), hi = maxim(1, n), mid;

    // Update the bounds to be off the target by a factor of 10
    while(100 * lo * lo < n) lo *= 10;
    while(0.01 * hi * hi > n) hi *= 0.1;

    for (int i = 0 ; i < 100 ; i++)
    {
        mid = (lo+hi)/2;
        if (mid*mid == n)
            return mid;
        if (mid*mid > n) 
            hi = mid;
        else 
            lo = mid;
    }
    return mid;
}

double absol(double x)
{
    return x > 0 ? x : -x;
}

void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void event_loop(SDL_Renderer* renderer, SDL_Texture* black_white, SDL_Texture* houghed)//SDL_Texture* flooded
{
    SDL_Event event;
    SDL_Texture* t = black_white;

    draw(renderer, t);
    
    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
		    return;

	    case SDL_WINDOWEVENT_RESIZED:
		    draw(renderer, t);
		    break;


        case SDL_KEYDOWN:
            if (t == black_white)
                t = houghed;
            else
                t = black_white;

            draw(renderer, t);
            break;
        }
    }
}


SDL_Surface* load_image(const char* path)
{
    SDL_Surface* tempSurface = IMG_Load(path);
    if (tempSurface == NULL)
    {
        printf("failed at making temp surface");
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tempSurface, SDL_PIXELFORMAT_RGB888, 0);
    if (surface == NULL)
    {
        printf("failed at making final surface");
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    SDL_FreeSurface(tempSurface);

    return surface;
}


void fill(Uint32* pixels, int x, int y, int w, int h, int *min_x, int *max_x, int *min_y, int *max_y, Uint32 gray)
{
    pixels[y * w + x] = gray;

    if (x > 0)
    {
        if (pixels[y * w + x - 1] > gray)
        {
            if (x < *min_x)
                *min_x -= 1;
            fill(pixels, x-1, y, w, h, min_x, max_x, min_y, max_y, gray);
        }
    }

    if (x < w - 1)
    {
        //printf("last : x < w - 1\n");
        //printf("w : %i\n", w);
        //printf("h : %i\n", h);
        //printf("x : %i\n", x);
        //printf("y : %i\n", y);
        if (pixels[y * w + x + 1] > gray)
        {
            //printf("entered\n");
            if (x > *max_x)
                *max_x += 1;
            fill(pixels, x+1, y, w, h, min_x, max_x, min_y, max_y, gray);
        }
    }

    if (y > 0)
    {
        if (pixels[(y-1) * w + x] > gray)
        {
            if (y < *min_y)
                *min_y -= 1;
            fill(pixels, x, y-1, w, h, min_x, max_x, min_y, max_y, gray);
        }
    }

    if (y < h - 1)
    {
        if (pixels[(y+1) * w + x] > gray)
        {
            if (y > *max_y)
                *max_y += 1;
            fill(pixels, x, y+1, w, h, min_x, max_x, min_y, max_y, gray);
        }
    }
}


void final_fill(Uint32* pixels, int x, int y, int w, int h, Uint32 gray, Uint32 white)
{
    pixels[y * w + x] = white;

    if (x > 0)
    {
        if (pixels[y * w + x - 1] == gray)
        {
            final_fill(pixels, x-1, y, w, h, gray, white);
        }
    }

    if (x < w - 1)
    {
        if (pixels[y * w + x + 1] == gray)
        {
            final_fill(pixels, x+1, y, w, h,gray, white);
        }
    }

    if (y > 0)
    {
        if (pixels[(y-1) * w + x] == gray)
        {
            final_fill(pixels, x, y-1, w, h, gray, white);
        }
    }

    if (y < h - 1)
    {
        if (pixels[(y+1) * w + x] == gray)
        {
            final_fill(pixels, x, y+1, w, h, gray, white);
        }
    }
}


void convert_to_flood(SDL_Surface* surface)
{
    SDL_PixelFormat* format = surface->format;
    Uint32* pixels = surface->pixels;
    Uint32 white = SDL_MapRGB(format, 255, 255, 255);
    Uint32 gray = SDL_MapRGB(format, 10, 10, 10);
    int w = surface->w;
    int h = surface->h;
    int locked = SDL_LockSurface(surface);
    if (locked != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    int biggest_x = 0;
    int biggest_y = 0;
    int biggest_area = 0;

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            int min_x = x;
            int max_x = x;
            int min_y = y;
            int max_y= y;

            if (pixels[y * w + x] > gray)
                fill(pixels, x, y, w, h, &min_x, &max_x, &min_y, &max_y, gray);

            if ((max_x - min_x) * (max_y - min_y)> biggest_area)
            {
                biggest_x = x;
                biggest_y = y;
                biggest_area = (max_x - min_x) * (max_y - min_y);
            }
        }
    }
    final_fill(pixels, biggest_x, biggest_y, w, h, gray, white);
    SDL_UnlockSurface(surface);
}

/*
int *hough(SDL_Surface* surface, int *res, int diag_l)
{
    //Applies hough transformation on the surface, puts the values for each rho and theta
    //in the matrix in parameter

    //condition : the matrix is in 90*diagonal length of surface (met on call)
    
    
    SDL_PixelFormat* format = surface->format;
    Uint32* pixels = surface->pixels;
    Uint32 gray = SDL_MapRGB(format, 10, 10, 10);
    int w = surface->w;
    int h = surface->h;
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            if (pixels[y*w + x] > gray)
            {
                for (int rho = 0; rho < diag_l; rho++)
                {
                    for (int theta = 0; theta < 11; theta++)
                    {
                        if (rho == (int)(y*cosine((double)theta*M_PI/180)) + (int)(x*sinus((double)theta*M_PI/180)))
                            res[theta*28 + rho] += 1;
                    }
                    for (int theta = 79; theta < 91; theta++)
                    {
                        if (rho == (int)(y*cosine((double)theta*M_PI/180)) + (int)(x*sinus((double)theta*M_PI/180)))
                            res[theta*28 + rho] += 1;
                    }
                }
            }
        }
    }
    return res;

}
*/



Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}


void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

void DrawLine_v3(SDL_Surface *img, int x0, int y0, int x1, int y1, float wd, Uint32 pixel_color)
{
	int dx = absol(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = absol(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx - dy, e2, x2, y2;
	float ed = dx + dy == 0 ? 1 : root((float) dx * dx + (float) dy * dy);


	Uint32 pixel = pixel_color;



	for (wd = (wd + 1) / 2 ; ; )
	{
		if (x0 >= 0 && y0 >= 0 && x0 < img -> h && y0 < img -> w)
		{
			put_pixel(img, y0, x0, pixel);
		}

		e2 = err;
		x2 = x0;

		if (2 * e2 >= -dx)
		{
			for (e2 += dy, y2 = y0; e2 < ed * wd && (y1 != y2 || dx > dy); e2 += dx)
			{
				if (x0 >= 0 && x0 < img -> h && (y2 + sy) >= 0 && (y2 + sy) < img -> w)
				{
					put_pixel(img, (y2 += sy), x0, pixel);
				}
			}

			if (x0 == x1)
			{
				break;
			}

			e2 = err;
			err -= dy;
			x0 += sx;
		}

		if (2 * e2 <= dy)
		{
			for (e2 = dx - e2; e2 < ed * wd && (x1 != x2 || dx < dy); e2 += dy)
			{
				if ((x2 + sx >= 0 && x2 + sx < img -> h) && (y0 >= 0 && y0 < img -> w))
				{
					put_pixel(img, y0, x2 += sx, pixel);
				}
			}

			if (y0 == y1)
			{
				break;
			}

			err += dx;
			y0 += sy;
		}


	}
}

/*
void drawlinesFromHough(SDL_Renderer* renderer, SDL_Surface* surface, int *matrix, int diag_l)
{
    //draws all lines in the surface that make at least 2/3 the size of it
    int threshold = (((surface->w + surface->h)/2)/3)*2;
    Uint32 color = SDL_MapRGB(surface->format, 255, 0, 0);
    for (int i = 0; i < (int)diag_l*90; i++)
    {
        if (matrix[i] > threshold)
        {
            //printf("found a value in matrix : %i\n", matrix[i]);
            int rho = i%28;
            int theta = i/28;

            if (absol(theta - 45) < 10 || absol(theta - 135) < 20) // skip diagonal lines
				continue;

			if (absol(theta - 90) > 10 && absol(theta) > 10) // skewed line => skip 
				continue;


			double a = cosine(theta * (M_PI / 180));
			double b = sinus(theta * (M_PI / 180));
				
			double x0 = a * rho; //+ surface->w / 2;
			double y0 = b * rho; //+ surface->h / 2;

			int x1 = (int) (x0 + 1000 * (-b));
			int y1 = (int) (y0 + 1000 * (a));
			int x2 = (int) (x0 - 1000 * (-b));
			int y2 = (int) (y0 - 1000 * (a));
			
            DrawLine_v3(surface, y1, x1, y2, x2, 0.03 * surface->w, color);
        }
    }
    
}
*/



void HoughTransform(SDL_Surface *edge_image, SDL_Surface *src, int num_thetas, int num_rhos, int threshold, struct line * lines)
{
	if (num_rhos == 0 || num_thetas == 0)
	{
		errx(1, "Errror Hough : incorrect parameters\n");
		return ;
	}

	int d = (int) (root((edge_image -> h) * (edge_image -> h) + (edge_image -> w) * (edge_image -> w)));

	double d_theta =  181 / num_thetas;
//	double d_rho = (int) (2 * d + 1) / num_rhos;

	double thetas[181];
	double sin_thetas[181];
	double cos_thetas[181];
	double rhos[2 * d + 1];

	int step = 0;
	for (int i = 0; i < 181; i++)
	{
		thetas[i] = step;

		sin_thetas[i] = sinus(thetas[i] * (M_PI / 180));
		cos_thetas[i] = cosine(thetas[i] * (M_PI / 180));
		step += d_theta;
	}

	step = -d;


	for (int i = 0; i < 2 * d + 1; i++)
	{
		rhos[i] = step + i;
	}

	printf("%f    %f\n", rhos[0], rhos[2 * d]);

	// init accumulator
	int accumulator[2 * d + 1][181];

	for (int i = 0; i < 2 * d + 1; i++)
	{
		for (int j = 0; j < 181; j++)
		{
			accumulator[i][j] = 0;
		}
	}


	// begin hough
	
	Uint32 pixel;
	Uint8 r, g, _b;

	int theta;
	double rho;
	
	int half_w = edge_image -> w / 2;
	int half_h = edge_image -> h / 2;

	int rho_index = 0;


	for (int y = 0; y < edge_image -> h; y++)
	{
		for (int x = 0; x < edge_image -> w; x++)
		{
			pixel = get_pixel(edge_image, x, y);
			SDL_GetRGB(pixel, src -> format, &r, &g, &_b);

			double edge_point_1 = x - half_w;
			double edge_point_2 = y - half_h;

			if (r == 255 && g == 255 && _b == 255) // white pixel => edge pixel
			{
				for (int k = 0; k < 181; k += d_theta)
				{
					rho = (edge_point_1 * cos_thetas[k]) + (edge_point_2 * sin_thetas[k]);

					theta = thetas[k];

					double min_rho_abs = DBL_MAX;

					for (int l = 0; l <  2 * d + 1; l++)
					{
						// get rho index 
						if (absol(rho - rhos[l]) < min_rho_abs)
						{
							rho_index = l;
							min_rho_abs = absol(rho - rhos[l]);
						}
						if (rhos[l] > rho) // reduce hough run time: rhos array in increasing order => break if current val > rho
							break;
					}

					accumulator[rho_index][theta]++;
				}
			}
		}
	}

	double a, b, x0, y0;

	double x1, y1, x2, y2;
	
    /*
	// set line color 
	Uint32 color;
	
	if (!strcmp(line_color, "red"))
	{
		color = SDL_MapRGB(src -> format, 255, 0, 0);
	}
	
	else if (!strcmp(line_color, "green"))
	{
		color = SDL_MapRGB(src -> format, 0, 255, 0);
	}
	
	else if (!strcmp(line_color, "blue"))
	{
		color = SDL_MapRGB(src -> format, 0, 0, 255);
	}
	
	else // white
	{
		color = SDL_MapRGB(src -> format, 255, 255, 255);
	}
    */

    Uint32 color;
    color = SDL_MapRGB(src -> format, 255, 0, 0);
	

    int currentLine = 0;
	for (int y = 0; y < 2 * d + 1; y++)
	{
		for (int x = 0; x < 181; x++)
		{
			if (accumulator[y][x] > threshold)
			{
				rho = rhos[y];
				theta = thetas[x];

				if (absol(theta - 45) < 15 || absol(theta - 135) < 20) // skip diagonal lines
					continue;

				if (absol(theta - 90) > 15 && absol(theta) > 15) // skewed line => skip 
					continue;

				a = cosine(theta * (M_PI / 180));
				b = sinus(theta * (M_PI / 180));
				
				x0 = (a * rho) + half_w;
				y0 = (b * rho) + half_h;

				x1 = (int) (x0 + 1000 * (-b));
				y1 = (int) (y0 + 1000 * (a));
				x2 = (int) (x0 - 1000 * (-b));
				y2 = (int) (y0 - 1000 * (a));

                //save line
                struct line line;
                line.x1 = x1;
                line.y1 = y1;
                line.x2 = x2;
                line.y2 = y2;
                line.angle = theta;
                line.flag = 1;
                
                lines[currentLine] = line;
                currentLine++;

				DrawLine_v3(src, y1, x1, y2, x2, 3, color);
			}
		}
	}
}

void get_inter(struct line line1, struct line line2, int* x, int* y)
{
    int x1 = line1.x1;
    int x2 = line1.x2;
    int x3 = line2.x1;
    int x4 = line2.x2;
    int y1 = line1.y1;
    int y2 = line1.y2;
    int y3 = line2.y1;
    int y4 = line2.y2;

    printf("x1 : %i\n", x1);
    printf("x2 : %i\n", x2);
    printf("x3 : %i\n", x3);
    printf("x4 : %i\n", x4);
    printf("y1 : %i\n", y1);
    printf("y2 : %i\n", y2);
    printf("y3 : %i\n", y3);
    printf("y4 : %i\n", y4);

    int u = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
    *x = x1 + u * (x2-x1);
    *y = y1 + u * (y2-y1);
}

void get_corners(struct line *lines, int *corners, int d)
{

    int minx = lines[0].x1;
    int miny = lines[0].y1;
    int minx_i = 0;
    int miny_i = 0;
    int maxx = lines[0].x1;
    int maxy = lines[0].y1;
    int maxx_i = 0;
    int maxy_i = 0;

    //get the border lines
    for (int i = 0; i < 2*d+1; i++)
    {
        if (lines[i].flag == 0)
            break;
        
        if (absol(lines[i].angle) > 75 && absol(lines[i].angle) < 105)
        {
            //is vertical
            if (lines[i].x1 < minx)
            {
                //is close to left
                minx = lines[i].x1;
                minx_i = i;
            }

            if (lines[i].x1 > maxx)
            {
                //close to right
                maxx = lines[i].x1;
                maxx_i = i;
            }
        }
        else
        {
            //is horizontal
            if (lines[i].y1 < miny)
            {
                //close to top
                miny = lines[i].y1;
                miny_i = i;
            }

            if (lines[i].y1 > maxy)
            {
                //close to bottom
                maxy = lines[i].y1;
                maxy_i = i;
            }
        }
    }

    //determines corners coords (TopL.x, TopL.y, TopR, BotL, BotR)
    int topLx = 0;
    int topLy = 0;
    int topRx = 0;
    int topRy = 0;
    int botLx = 0;
    int botLy = 0;
    int botRx = 0;
    int botRy = 0;

    printf("trying get inter \n");
    printf("maxx_i : %i\n" ,maxx_i);
    printf("miny_i : %i\n" ,miny_i);

    //topL
    get_inter(lines[minx_i], lines[miny_i], &topLx, &topLy);
    //topR
    get_inter(lines[maxx_i], lines[miny_i], &topRx, &topRy);
    //botL
    get_inter(lines[minx_i], lines[maxy_i], &botLx, &botLy);
    //botR
    get_inter(lines[maxx_i], lines[maxy_i], &botRx, &botRy);

    corners[0] = topLx;
    corners[1] = topLy;
    corners[2] = topRx;
    corners[3] = topRy;
    corners[4] = botLx;
    corners[5] = botLy;
    corners[6] = botRx;
    corners[7] = botRy;
}

void get_cells(int *corners, SDL_Rect *cells)
{
    int x = corners[0];
    int y = corners[1];
    int w = corners[2] - corners[0];

    for (int i = 0; i < 8; i++)
    {
        if (i == 4)
            w = corners[6] - corners[4];

        int h = corners[5] - corners[1];

        for (int j = 0; i < 8; i++)
        {
            if (j == 4)
            {
                h = corners[7] - corners[3];
            }
            SDL_Rect rect = {x, y, w/8, h/8};
            cells[j * 8 + i] = rect;
            x += w/8;
        }
        y += h/8;
    }
}




int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // TODO:
    const char* path = argv[1];

    // - Initialize the SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
	    errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a window.
    SDL_Window* window = SDL_CreateWindow("Image Display", 0, 0, 640, 400, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
	    errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
	    errx(EXIT_FAILURE, "%s", SDL_GetError());

    //draw lines on renderer


    // - Create a surface from the colored image.
    SDL_Surface* surface = load_image(path);


    // - Resize the window according to the size of the image.
    SDL_SetWindowSize(window, surface->w, surface->h);

    // - Create a texture from the colored surface.
    SDL_Texture* black_white = SDL_CreateTextureFromSurface(renderer, surface);
    // - Convert the surface into grayscale.
    convert_to_flood(surface);
    SDL_Surface* surface_copy = load_image(path);
    convert_to_flood(surface_copy);

    // - Create a new texture from the grayscale surface.
    //SDL_Texture* flooded = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Surface* edgeimage = SDL_ConvertSurface(surface, surface->format, 0);

    double diag_l = root((double)(surface->w * surface->w + surface->h * surface->h));
    struct line lines[2*(int)diag_l+1];
    for (int i = 0; i < 2*(int)diag_l+1; i++)
    {
        lines[i].x1 = 0;
        lines[i].y1 = 0;
        lines[i].x2 = 0;
        lines[i].y2 = 0;
        lines[i].angle = 0;
        lines[i].flag = 0;
    }
    
    /*
    for (int i = 0; i < 2*(int)diag_l+1; i++)
    {
        lines[i] = NULL;
    }
    */
    HoughTransform(edgeimage, surface, 180, 180, (surface->w /2 + surface->h /2)/2, lines);

    SDL_Texture* houghed = SDL_CreateTextureFromSurface(renderer, surface);

    //detect corners

    /*
    int corners[8] = {0};
    get_corners(lines, corners, (int)diag_l);*/
/*
    for (int i = 0; i < 8; i++)
    {
        printf("%i\n", corners[i]);
    }
*/

    SDL_Rect cells[64];
    //get_cells(corners, cells);
    IMG_SavePNG(surface, "final_picture.png");


    // - Free the surface.
    SDL_FreeSurface(surface);

    // - Dispatch the events.
    event_loop(renderer, black_white, houghed); //flooded

    // - Destroy the objects.
    SDL_DestroyTexture(black_white);
    //SDL_DestroyTexture(flooded);
    SDL_DestroyTexture(houghed);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

/*
    int test[28 * 91] = {};
    int *res;
    res = hough(test);
    for (int i = 0; i < 28*91; i++)
    {
        if (res[i] > 7)
            printf("found : res[%i][%i] = %i\n", i%28, i/28, res[i]);
    }
*/

    

    return EXIT_SUCCESS;
}
