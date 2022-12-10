#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./include/parser.h"
#include "./include/display.h"
#include "./include/solver.h"

int main(int argc, char** argv)
{
    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: ./solver \"sudoku-path\" width");

    int max = atoi(argv[2]) * atoi(argv[2]);
    int *sudok = malloc(max * sizeof(int));
    parser(argv[1], sudok);
    display_sudoku(sudok, atoi(argv[2]));
    printf("\n--- SOLVING SUDOKU ---\n\n");

    //Make a copy of the sudoku to solve it
    int *sudokInitial = malloc(max * sizeof(int));
    memcpy(sudokInitial, sudok, max * sizeof(int));
    
    if (solveSudoku(sudok, 0, 0, atoi(argv[2])) == 0)
        errx(EXIT_FAILURE, "No solution exists for the sudoku.");
    display_sudoku(sudok, atoi(argv[2]));
    extract_sudoku(sudok, sudokInitial, max);
    
    return EXIT_SUCCESS;
}
