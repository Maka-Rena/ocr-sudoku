#define MAX_SIZE 81
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include "./include/parser.h"
#include "./include/display.h"
#include "./include/solver.h"

int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: ./solver \"sudoku-path\"");

    int *sudok = malloc(MAX_SIZE * sizeof(int));
    parser(argv[1], sudok);
    display_sudoku(sudok);
    printf("\n--- SOLVING SUDOKU ---\n\n");
    if (solveSudoku(sudok, 0, 0) == 0)
        errx(EXIT_FAILURE, "No solution exists for the sudoku.");
    display_sudoku(sudok);

    return EXIT_SUCCESS;
}
