#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "../include/display.h"

void display_sudoku(int* sudok, int n)
{
    if (n == 9)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                printf("%d", sudok[i*n+j]);
                if (j == 2 || j == 5)
                    printf(" ");
            }
            printf("\n");
            if (i == 2 || i == 5)
                printf("\n");
        }
    }
    else{
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                printf("%d", sudok[i*n+j]);
                if (j == 3 || j == 7 || j == 11)
                    printf(" ");
            }
            printf("\n");
            if (i == 3 || i == 7 || i == 11)
                printf("\n");
        }
    }
}

void extract_sudoku(int* sudok, int* sudokInitial, int max)
{
    //Create a JSON file with an array of the solved sudoku with two colors for an initial number and a solved number
    FILE *f = fopen("../../../web/website/src/components/Upload/process/solvedSudoku.json", "w");
    if (f == NULL)
        errx(EXIT_FAILURE, "Error opening file!");
    fprintf(f, "{\n\t\"sudoku\": [\n");
    for (int i = 0; i < max; i++)
    {
        if (sudokInitial[i] == 0)
            fprintf(f, "\t\t{\"value\": %d, \"solved\": \"true\"}", sudok[i]);
        else
            fprintf(f, "\t\t{\"value\": %d, \"solved\": \"false\"}", sudok[i]);
        if (i != max - 1)
            fprintf(f, ",\n");
        else
            fprintf(f, "\n");
    }
    fprintf(f, "\t]\n}");
    fclose(f);
}
