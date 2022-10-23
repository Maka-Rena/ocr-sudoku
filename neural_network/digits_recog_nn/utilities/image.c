#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 10000

void img_print(Image* img)
{
    matrix_print(img->img_data);
    printf("Image Label: %d\n", img->label);
}

void img_free(Image* img)
{
    matrix_free(img->img_data);
    free(img);
    img = NULL;
}

void imgs_free(Image** imgs, int n)
{
    for(int i = 0; i < n; i++)
        img_free(imgs[i]);
}
