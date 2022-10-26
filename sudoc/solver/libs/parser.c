#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "../include/parser.h"

void parser (char* file_path, int* sudok)
{
    FILE *file = fopen(file_path, "r");
    size_t n = 0;
    int c;

    if (file == NULL)
       errx(EXIT_FAILURE, "File not found!");

    while ((c = fgetc(file)) != EOF)
    {
        switch (c)
        {
            case '.':
                sudok[n++] = 0;
                break;
            case '\n':
                break;
            case ' ':
                break;
            default:
                sudok[n++] = c - '0';
        }
    }
}