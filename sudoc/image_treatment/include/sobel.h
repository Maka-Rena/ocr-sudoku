#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

float find_max(float list[], int len);

void __sobel_transformation(SDL_Surface* surface, Uint32* resultgradient); //, Uint32* resultangle);

void Kernel_Convolution_Sobel(SDL_Surface* surface);
