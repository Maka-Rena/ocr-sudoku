#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


void draw(SDL_Renderer* renderer, SDL_Texture* texture);

void event_loop(SDL_Renderer* renderer, SDL_Texture* colored, SDL_Texture* texture_gray, SDL_Texture* texture_blurred, SDL_Texture* texture_sobel);

SDL_Surface* Load_image(const char* path);

Uint32 __Pixel_To_Grayscale(Uint32 pixel_color, SDL_PixelFormat* format);

void Surface_To_Grayscale(SDL_Surface* surface);
