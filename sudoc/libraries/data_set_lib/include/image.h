#pragma once

#include "../../matrix_lib/include/matrix.h"

typedef struct
{
    Matrix* img_data;
    int label;
} Image;

Image** csv_to_imgs(char* file_string, int number_of_imgs);
void img_print(Image* img);
void img_free(Image* img);
void imgs_free(Image** imgs, int n);
