#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <err.h>
#include <string.h>
#include "include/neural_network.h"
#include "../../libraries/matrix_lib/include/matrix.h"
#include "../../libraries/matrix_lib/include/ops.h"
#include "include/activation.h"
#include "include/load_and_save.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Matrix *lower(SDL_Surface* surface)
{

    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    //le format
    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    Matrix* res = matrix_create(28,28);
    for (int i = 0;i < surface->h;i++)
    {
        for (int j =0; j < surface->w;j++)
        { 
            Uint8 r,g,b;
            SDL_GetRGB(pixels[i*surface->w + j], format, &r, &g, &b);
            // get value of pixel on scale 0-255
            //float av = ((float)(r+g+b))/3.0;
            float av = 0.3*r + 0.59*g + 0.11*b;
            /*if (r == 0)
            {
                av = 0;
            }
            else
            {
                av = 255;
            }*/
            
            res->entries[j][i] = av;
        }
    }
    //Matrix* res2 = apply(sigmoid, res);
    return res;
}

SDL_Surface* Load_image(const char* path)
{
    SDL_Surface* surface = IMG_Load(path);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface* res = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGB888,0);
    if (res == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(surface);
    return res;
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    if(argc > 3 || argc < 1)
        errx(1, "Wrong number of arguments !");
    if(strcmp(argv[1], "train") == 0)
        // TRAINING
    {
        if(strcmp((argv[2]), "handwritten") == 0)
        {
            int number_imgs = 60000;
            Image** imgs = csv_to_imgs("../../libraries/data_set_lib/DataSet/mnist_train.csv", number_imgs);
            NeuralNetwork* net = network_create(784, 30, 10, 0.06f);
            network_train_batch_imgs(net, imgs, number_imgs,10);
            network_save(net, "testing_net");
            network_print(net);
            
            imgs_free(imgs,number_imgs);
            network_free(net);

        }
        if(strcmp((argv[2]), "computer") == 0)
        {
            int number_imgs = 39900;
            Image** imgs = csv_to_imgs("../../libraries/data_set_lib/DataSet/computer_train_40000_shuffle.csv", number_imgs);
            NeuralNetwork* net = network_create(784, 40, 10, 0.075f);
            network_train_batch_imgs(net, imgs, number_imgs,10);
            network_save(net, "computer_net");
            network_print(net);
            
            imgs_free(imgs,number_imgs);
            network_free(net);

        }
        if(strcmp((argv[2]), "hexa") == 0)
        {
            int number_imgs = 62700;
            Image** imgs = csv_to_imgs("../../libraries/data_set_lib/DataSet/computer_train_hexa_shuffle.csv", number_imgs);
            NeuralNetwork* net = network_create(784, 25, 17, 0.15f);
            network_train_batch_imgs(net, imgs, number_imgs,17);
            network_save(net, "computer_net_hexa");
            network_print(net);
            
            imgs_free(imgs,number_imgs);
            network_free(net);

        }


    }
    // PREDICTING
    if(strcmp((argv[1]), "predict") == 0)
    {
        if(strcmp((argv[2]), "handwritten") == 0)
        {
            int number_imgs = 10000;
            Image** imgs = csv_to_imgs("../../libraries/data_set_lib/DataSet/mnist_test.csv", number_imgs);
            NeuralNetwork* net = network_load("testing_net");
            // NeuralNetwork* net = network_load("handwritten_test");
            printf("On est ok !\n");
            double score = network_predict_imgs(net, imgs, number_imgs);
            
            imgs_free(imgs,number_imgs);
            printf("Score for handwritten characters : %1.5f\n", score);
            network_free(net);

        }
        else if(strcmp((argv[2]), "computer") == 0)
        {
            int number_imgs = 14100;
            Image** imgs = csv_to_imgs("../../libraries/data_set_lib/DataSet/computer_test_14000_shuffle.csv", number_imgs);
            NeuralNetwork* net = network_load("computer_net");
            printf("On est ok !\n");
            double score = network_predict_imgs(net, imgs, number_imgs);

            printf("Score for computer characters : %1.5f\n", score);
            
            imgs_free(imgs,number_imgs);
            network_free(net);


        }

        else if(strcmp((argv[2]), "hexa") == 0)
        {
            int number_imgs = 30500;
            Image** imgs = csv_to_imgs("../../libraries/data_set_lib/DataSet/computer_test_hexa_shuffle.csv", number_imgs);
            NeuralNetwork* net = network_load("computer_net_hexa");
            printf("On est ok !\n");
            double score = network_predict_imgs(net, imgs, number_imgs);

            printf("Score for hexa characters : %1.5f\n", score);
            
            imgs_free(imgs,number_imgs);
            network_free(net);


        }

    }

    if(strcmp((argv[1]), "test") == 0)
    {
        SDL_Surface *surface = Load_image("../../../pictures/28x28.png");
        NeuralNetwork* net = network_load("computer_net");
        Matrix *to_img = lower(surface);

        Image *my_img = malloc(sizeof(Image));
        my_img->img_data = to_img;
        my_img->label = 6;
        printf("heyhey\n");
        Matrix *res = network_predict_img(net,my_img);
        printf("predicted number = %i\n", matrix_argmax(res));
        printf("actual number = %i\n", 6);
        network_free(net);
        /*matrix_print(imgs[5]->img_data); 
          Matrix* res = network_predict_img(net,imgs[5]);
          printf("predicted number = %i\n", matrix_argmax(res));
          printf("actual number = %i\n", imgs[5]->label);
          imgs_free(imgs, number_imgs);*/
    }

    if(strcmp((argv[1]), "process") == 0)
    {
        int processed_sudoku[81] = {0}; 
        // for each image in directory load it and process it
        for(int i = 0; i < 81; i++)
        {
            NeuralNetwork* net = network_load("computer_net");
            char* path = malloc(sizeof(char)*100);
            sprintf(path, "../images/%i.png", i);
            SDL_Surface *surface = Load_image(path);
            Matrix *to_img = lower(surface);
            Image *my_img = malloc(sizeof(Image));
            my_img->img_data = to_img;
            Matrix *res = network_predict_img(net,my_img);
            int number = matrix_argmax(res);
            processed_sudoku[i] = number;
            printf("predicted number = %i\n", number);
            network_free(net);
            img_free(my_img);
            // save it in a csv file
        }
        // pretty print of array
        /*for(int i = 0; i < 9; i++)
        {
            for(int j = 0; j < 9; j++)
            {
                printf("%i ", processed_sudoku[i*9+j]);
            }
            printf("\n");
        }*/
        // print array
        for(int i = 0; i < 81; i++)
        {
            char j = '.';
            if (processed_sudoku[i] == 0)
            {
                printf("%c,", j);
            }
            else
            {
                printf("%i,", processed_sudoku[i]);
            }
            if (i%9 == 8)
            {
                printf("\n");
            }
            
        }

    }
    return 0;
}
