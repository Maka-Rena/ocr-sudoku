#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>

void rotate_image(SDL_Surface *image, SDL_Surface *rotated, float teta);