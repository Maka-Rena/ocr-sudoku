#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "../../libraries/matrix_lib/include/matrix.h"
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


int pixel_to_bin(Uint32 pixel_color, SDL_PixelFormat* format);
SDL_Surface* load_image(const char* path);
Matrix* surface_to_matrix(SDL_Surface* surface);

