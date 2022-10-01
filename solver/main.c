#include <err.h>
#include <stdio.h>
#include "./include/parser.h"
#include "./include/display.h"
#include "./include/solver.h"
#define MAX_SIZE 81
int main(int argc, char** argv)
{
    if (argc != 2)
        return 1;//errx(EXIT_FAILURE, "Usage: solver sudoku-path");

    char sudok[MAX_SIZE];
    sudok = parser(argv[1]);
    printf("%s\n", sudok);
    return 0;
}
