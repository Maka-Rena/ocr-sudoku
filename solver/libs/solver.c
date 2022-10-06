#include "../include/solver.h"
#define N 9


int isAvailable(int puzzle[], int row, int col, int num)
{
    int rowStart = (row/3) * 3;
    int colStart = (col/3) * 3;
    int i;

    for(i=0; i<9; ++i)
    {
        if (puzzle[row*N+i] == num) return 0;
        if (puzzle[i*N+col] == num) return 0;
        if (puzzle[(rowStart + (i%3))*N+(colStart + (i/3))] == num) return 0;
    }
    return 1;
}

int solveSudoku(int *sudok, int row, int col)
{
    int i;
    if(row<9 && col<9)
    {
        if(sudok[row*N+col])
        {
            if((col+1)<9) return solveSudoku(sudok, row, col+1);
            else if((row+1)<9) return solveSudoku(sudok, row+1, 0);
            else return 1;
        }
        else
        {
            for (i=0; i<9; ++i)
            {
                if (isAvailable(sudok, row, col, i+1))
                {
                    sudok[row*N+col] = i+1;
                    if (solveSudoku(sudok, row, col))
                        return 1;
                    else
                        sudok[row*N+col] = 0;
                }
            }
        }
        return 0;
    }
    else return 1;
}