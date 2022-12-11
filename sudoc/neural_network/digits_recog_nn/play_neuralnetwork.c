#include "play_neuralnetwork.h"

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

SDL_Surface* Load_image2(const char* path)
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

void play_neuralnetwork(int type, int N, int processed_sudoku[])
{    
    NeuralNetwork* net = malloc(sizeof(NeuralNetwork));
    if (type == 0) //HANDWRITTEN
    {
        net = network_load("./sudoc/neural_network/digits_recog_nn/testing_net");
    }
    else if (type == 1) //COMPUTER
    {
        net = network_load("./sudoc/neural_network/digits_recog_nn/computer_net");
    }
    else{ // HEXA
        net = network_load("./sudoc/neural_network/digits_recog_nn/computer_net_hexa");
    }
    for(int i = 0; i < N*N; i++)
    {
        char* path = malloc(sizeof(char)*100);
        sprintf(path, "../images/%i.png", i);
        SDL_Surface *surface = Load_image2(path);
        Matrix *to_img = lower(surface);
        Image *my_img = malloc(sizeof(Image));
        my_img->img_data = to_img;
        Matrix *res = network_predict_img(net,my_img);
        int number = matrix_argmax(res);
        processed_sudoku[i] = number;
        img_free(my_img);
    }
    network_free(net);
    return;
}