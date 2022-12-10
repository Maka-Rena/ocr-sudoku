#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void croped_image(SDL_Surface *image, SDL_Surface *croped, int x1, int y1, int x2, int y2);
void Clean_surface(SDL_Surface *surface);