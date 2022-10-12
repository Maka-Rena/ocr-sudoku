#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include "./include/to_matrix.h"
#include "../libraries/matrix_lib/matrix.h"
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./include/cell.h"

int main(int argc, char** argv)
{
    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    // - Create a surface from the colored image.
    SDL_Surface* surface = load_image(argv[1]);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    Matrix* matrix = surface_to_matrix(surface);
    matrix_print(matrix);
    EXIT_SUCCESS;
}
