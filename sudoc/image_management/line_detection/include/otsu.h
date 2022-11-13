#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


float Otsu_Threshold(Uint32* pixels, int len, SDL_PixelFormat* format);
void otsu(SDL_Surface* surface);