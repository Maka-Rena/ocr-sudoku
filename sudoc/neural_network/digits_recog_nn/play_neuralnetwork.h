#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <err.h>
#include <string.h>
#include "include/neural_network.h"
#include "../../libraries/matrix_lib/include/matrix.h"
#include "../../libraries/matrix_lib/include/ops.h"
#include "include/activation.h"
#include "include/load_and_save.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Matrix *lower(SDL_Surface* surface);
SDL_Surface* Load_image2(const char* path);
void play_neuralnetwork(int type, int N, int processed_sudoku[N*N]);