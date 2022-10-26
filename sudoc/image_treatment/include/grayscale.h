#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


void __Pixel_To_Grayscale(Uint32 pixel_color, SDL_PixelFormat* format, Uint32 *color, uint *average);

void Surface_To_Grayscale(SDL_Surface* surface);
