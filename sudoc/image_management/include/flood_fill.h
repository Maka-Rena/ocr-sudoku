#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void flood_fill(Uint32 *pixels, SDL_PixelFormat *format,
                Uint32 color, int i, int j, int w, int h, int* number_of_pixels);

void flood_fill_final(Uint32 *pixels, SDL_PixelFormat *format,
                Uint32 color, int i, int j, int w, int h);      

int* get_coordinates(Uint32 *pixels, Uint32 color, int len);  

void bringing_back(SDL_Surface *image);
void deleting(SDL_Surface* surface);

int *princip(SDL_Surface* surface);