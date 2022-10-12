#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "../../libraries/matrix_lib/matrix.h"
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


int pixel_to_bin(Uint32 pixel_color, SDL_PixelFormat* format);

