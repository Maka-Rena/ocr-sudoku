#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "../include/display.h"
#define N 9

void display_sudoku(int* sudok)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d", sudok[i*N+j]);
            if (j == 2 || j == 5)
                printf(" ");
        }
        printf("\n");
        if (i == 2 || i == 5)
            printf("\n");
    }
}
