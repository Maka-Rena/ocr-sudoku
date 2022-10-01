#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "../include/display.h"

void display_sudoku(char* s[], size_t size, char* file_name[])
{
    //Openning file to write the Sudoku
    FILE *file = fopen(file_name, "w+");

    for (size_t row = 0; row < size; row++)
    {
        for (size_t col = 0; col < size; col++)
        {
            if (s[size*row + col] == )
        }
    }
}
