#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <err.h>
#include <string.h>
#include "include/neural_network.h"
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
            float av = ((float)(r+g+b))/3;
            SDL_GetRGB(pixels[i*surface->w + j], format, &r, &g, &b);
            //if(r == 255)
                res->entries[i][j] = av;
            //else
                res->entries[i][j] = av;
        }
    }
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
    if(argc > 2 || argc < 1)
        errx(1, "Wrong number of arguments !");
    if(strcmp(argv[1], "train") == 0)
        // TRAINING
    {
        int number_imgs = 60000;
        Image** imgs = csv_to_imgs("../../libraries/data_set_lib/DataSet/mnist_train.csv", number_imgs);
        NeuralNetwork* net = network_create(784, 30, 10, 0.06f);
        network_train_batch_imgs(net, imgs, number_imgs);
        network_save(net, "testing_net");
        network_print(net);

        network_free(net);
    }
    // PREDICTING
    if(strcmp((argv[1]), "predict") == 0)
    {
        int number_imgs = 10000;
        Image** imgs = csv_to_imgs("../../libraries/data_set_lib/DataSet/mnist_test.csv", number_imgs);
        NeuralNetwork* net = network_load("testing_net");
        printf("On est ok !\n");
        double score = network_predict_imgs(net, imgs, 10000);
        
        printf("Score: %1.5f\n", score);
        
        network_free(net);
    }

    if(strcmp((argv[1]), "test") == 0)
    {
        SDL_Surface *surface = Load_image("../../../pictures/28x28.png");
        NeuralNetwork* net = network_load("testing_net");
        Matrix *to_img = lower(surface);
        
        printf("heyho\n");
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
    return 0;
}
