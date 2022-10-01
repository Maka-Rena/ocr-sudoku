#include <stdio.h>
#include <stdlib.h>
#include <err.h>

char parser (char file_path)
{
    FILE *file = fopen(file_path, "r");
    char *arr;
    size_t n = 0;
    int c;

    if (file == NULL)
       errx(EXIT_FAILURE, "File not found!");

    // 256 if 16*16 of Sudoku Size
    arr = malloc(256 * sizeof(char));

    while ((c = fgetc(file)) != EOF)
    {
        switch (c)
        {
            case '.':
                arr[n++] = '0';
                break;
            case '\n':
                break;
            default:
                if ('0' < c && c >= '9')
                    arr[n++] = (char) c;
                else
                    errx(EXIT_FAILURE, "Char not known");
        }
    }
    return arr;
}
