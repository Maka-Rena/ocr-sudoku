#include "../include/solver.h"
#include "../include/display.h"

int isAvailable(int puzzle[], int row, int col, int num, int n)
{
    //ONLY N = 9 works
    int rowStart = (row/3) * 3;
    int colStart = (col/3) * 3;
    int i;

    for(i=0; i<n; ++i)
    {
        if (puzzle[row*n+i] == num) return 0;
        if (puzzle[i*n+col] == num) return 0;
        if (puzzle[(rowStart + (i%3))*n+(colStart + (i/3))] == num) return 0;
    }
    return 1;
}

int solveSudoku(int *sudok, int row, int col, int n)
{
    //ONLY N = 9 works
    if(row<n && col<n)
    {
        if(sudok[row*n+col])
        {
            if((col+1)<n) return solveSudoku(sudok, row, col+1, n);
            else if((row+1)<n) return solveSudoku(sudok, row+1, 0, n);
            else return 1;
        }
        else
        {
            for (int i=0; i<n; ++i)
            {
                if (isAvailable(sudok, row, col, i+1, n))
                {
                    sudok[row*n+col] = i+1;
                    if (solveSudoku(sudok, row, col, n))
                        return 1;
                    else
                        sudok[row*n+col] = 0;
                }
            }
        }
        return 0;
    }
    else return 1;
}