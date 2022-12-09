#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "crop.h"
#include "resize.h"

void crop_9(SDL_Surface* surface, int* coordinates);