#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void blackandwhite(SDL_Surface* Surface);
double pw2(double x);
int threshold(Uint32* pixels, int length, SDL_PixelFormat* format);
void thinning(Uint32* pixels, SDL_PixelFormat* format, int w, int h);