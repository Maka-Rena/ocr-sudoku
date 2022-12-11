#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../../libraries/matrix_lib/include/matrix.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./hough_transform.h"
#include "./black_n_white.h"
#include "./cell.h"
#include "./contrast_light.h"
#include "./crop_ia.h"
#include "./crop.h"
#include "./gaussian.h"
#include "./grayscale.h"
#include "./resize.h"
#include "./sobel.h"
#include "./to_matrix.h"
#include "./flood_fill.h"

SDL_Surface* Load_image(const char* path);
void play_treatment(char* path);