#include "../include/parserTests.h"

int parserTests ()
{
    int sudok[81];
    parser("tests/samples/sudoku01.txt", sudok);
    int sudokTest[81] = {0};
    for (int i = 0; i < 81; i++)
    {
        if (sudok[i] != sudokTest[i])
            return 1;
    }
    return 0;
}