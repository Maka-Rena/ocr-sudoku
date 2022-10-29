#include "../include/parserTests.h"

int parserTests_1()
{
    int sudok1[81];
    parser("tests/samples/sudoku01.txt", sudok1);
    int sudokTest[81] = {0};
    for (int i = 0; i < 81; i++)
    {
        if (sudok1[i] != sudokTest[i])
            return 1;
    }
    return 0;
}
int parserTests_2()
{
    int sudok[81];
    parser("tests/samples/sudoku02.txt", sudok);
    int sudokTest[81] = {0};
    sudokTest[0] = 2;
    sudokTest[4] = 8;
    sudokTest[9*1+7] = 4;
    sudokTest[9*4+0] = 1;
    sudokTest[9*6+5] = 8;
    for (int i = 0; i < 81; i++)
    {
        if (sudok[i] != sudokTest[i])
        {
            printf("FAILED AT i = %d\n", i);
            return 1;
        }
    }
    return 0;
}