#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

double find_max(double list[], int len);

void __sobel_transformation(SDL_Surface* surface, double* resultgradient); //, Uint32* resultangle);

void Kernel_Convolution_Sobel(SDL_Surface* surface);
int threshold_sob(Uint32* pixels, int length, SDL_PixelFormat* format);