#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

double bilinearly_interpolate(unsigned int top, unsigned int bottom,
                              unsigned int left, unsigned int right,
                              double horizontal_position,
                              double vertical_position, Uint32* pixels, int width, SDL_PixelFormat* format);
SDL_Surface* resize(SDL_Surface* surface, unsigned int newwidth, unsigned int newheight);