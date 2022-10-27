#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void __Blurring_process(SDL_Surface* surface, double* result);

void Kernel_Convolution(SDL_Surface* surface);
