#include "../include/solverTests.h"
#define N 9

int isSudokValid(int* sudoc)
{
    for (int col = 0; col < N; col++)
    {
        for (int row = 0; row < N; row++)
        {
            if (sudoc[col*N+row] == 0)
                return 1;
        }
    }
    return 0;
}

int solverTests_1()
{
    int sudok[N*N];
    parser("tests/samples/sudoku01.txt", sudok);
    return isSudokValid(sudok);
}

int solverTests_2()
{
    int sudok[N*N];
    parser("tests/samples/sudoku02.txt", sudok);
    return isSudokValid(sudok);
}