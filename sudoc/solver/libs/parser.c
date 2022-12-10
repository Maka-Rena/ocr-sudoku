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
                if (c < '0' || c > '9')
                {
                    switch (c)
                    {
                    case 'A':
                        sudok[n++] = 10;
                        break;

                    case 'B':
                        sudok[n++] = 11;
                        break;
                    
                    case 'C':
                        sudok[n++] = 12;
                        break;

                    case 'D':
                        sudok[n++] = 13;
                        break;

                    case 'E':
                        sudok[n++] = 14;
                        break;

                    case 'F':
                        sudok[n++] = 15;
                        break;
                    
                    default:
                        errx(EXIT_FAILURE, "Invalid character in file!");
                        break;
                    }
                }
                else
                    sudok[n++] = c - '0';
        }
    }
}