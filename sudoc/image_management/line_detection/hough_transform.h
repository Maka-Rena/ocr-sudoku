#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include <err.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <math.h>
#define M_PI 3.14159265358979323846


double cosine(double x);

double sinus(double x);

double atang2(double x);

double minim(double a, double b);

double maxim(double a, double b);

double root(double n);

double absol(double x);

void draw(SDL_Renderer* renderer, SDL_Texture* texture);

void event_loop(SDL_Renderer* renderer, SDL_Texture* black_white, SDL_Texture* flooded);


SDL_Surface* load_image(const char* path);

void fill(Uint32* pixels, int x, int y, int w, int h, int *min_x, int *max_x, int *min_y, int *max_y, Uint32 gray);


void final_fill(Uint32* pixels, int x, int y, int w, int h, Uint32 gray, Uint32 white);
void convert_to_flood(SDL_Surface* surface);

Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

void DrawLine_v3(SDL_Surface *img, int x0, int y0, int x1, int y1, float wd, Uint32 pixel_color);

void HoughTransform(SDL_Surface *edge_image, SDL_Surface *src, int num_thetas, int num_rhos, int threshold, struct line * lines);

void get_inter(struct line line1, struct line line2, int* x, int* y);

void get_corners(struct line *lines, int *corners, int d);

void get_cells(int *corners, SDL_Rect *cells);

int main(int argc, char** argv);

