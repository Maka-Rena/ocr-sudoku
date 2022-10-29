#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


void draw_lines(SDL_Surface* surface, int x1, int y1, int x2, int y2, SDL_Renderer* renderer);

void hough_transform(SDL_Surface* surface, double* resultangle, SDL_Renderer* renderer);

double degrees_ToRadians(double degrees);

double radian_To_Degree(double radian);