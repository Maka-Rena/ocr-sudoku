#define MAX_SIZE 81
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include "./include/parser.h"
//#include "./include/display.h"
//#include "./include/solver.h"

int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: solver sudoku-path");

    int *sudok = malloc(MAX_SIZE * sizeof(int));
    parser(argv[1], sudok);

    for (size_t i = 0; i < 81; i++)
    {
        printf("%d", sudok[i]);
        if (i+1 % 9 == 0)
            printf("\n");
    }
    

    return EXIT_SUCCESS;
}
