#include "../include/image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 10000

Image** csv_to_imgs(char* file_string, int number_of_imgs) 
{
    FILE *fp;
    Image** imgs = malloc(number_of_imgs * sizeof(Image*));
    char row[MAXCHAR];
    fp = fopen(file_string, "r");

    //Read the first line 
    if(fgets(row, MAXCHAR, fp)){};
    int i = 0;
    while (feof(fp) != 1 && i < number_of_imgs)
    {
        imgs[i] = malloc(sizeof(Image));

        int j = 0;
        if(fgets(row, MAXCHAR, fp)){};
        char* token = strtok(row, ",");
        imgs[i]->img_data = matrix_create(28,28);
        while(token != NULL)
        {
            if(j == 0)
            {
                imgs[i]->label = atoi(token);
            }
            else
            {
                imgs[i]->img_data->entries[(j-1)/28][(j-1)%28] = atoi(token)/256.0;
            }
            token = strtok(NULL,",");
            j++;
        }
        i++;
    }
    fclose(fp);
    return imgs;
}

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
