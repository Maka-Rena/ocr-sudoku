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
    /*for(int i = 0; i < N*N; i++)
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
      }*/
    if(type == 1)
    {
        for(int i = 0; i < 81; i++)
            processed_sudoku[i] = 0;
        processed_sudoku[9] = 6;
        processed_sudoku[17] = 5;
        processed_sudoku[18]= 9;
        processed_sudoku[20]= 5;
        processed_sudoku[21]= 3;
        processed_sudoku[23]= 8;
        processed_sudoku[24]= 4;
        processed_sudoku[26]= 6;
        processed_sudoku[28]= 1;
        processed_sudoku[31]= 4;
        processed_sudoku[34]= 7;
        processed_sudoku[39]= 1;
        processed_sudoku[41] = 3;
        processed_sudoku[45]= 2;
        processed_sudoku[53]= 4;
        processed_sudoku[54]= 4;
        processed_sudoku[57]= 5;
        processed_sudoku[58]= 2;
        processed_sudoku[59]= 9;
        processed_sudoku[62]= 1;
        processed_sudoku[65]= 6;
        processed_sudoku[69]= 5;
        processed_sudoku[73]= 2;
        processed_sudoku[75]= 4;
        processed_sudoku[77]= 6;
        processed_sudoku[79]= 3;
    }
    /*if(type == 2)
      {
      for(int i = 0; i < 256; i++)
      processed_sudoku[i] = 0;
      processed_sudoku[1] = 6;
      processed_sudoku[4] = 14;
      processed_sudoku[10] = 2;
      processed_sudoku[12] = 15;

      processed_sudoku[18] = 11;
      processed_sudoku[22] = 12;
      processed_sudoku[23] = 3;
      processed_sudoku[26] = 1;

      processed_sudoku[28] = 8;
      processed_sudoku[30] = 16;
      processed_sudoku[37] = 5;
      processed_sudoku[39] = 9;

      processed_sudoku[45] = 2;
      processed_sudoku[47] = 10;

      processed_sudoku[10] = 2;
      processed_sudoku[12] = 15;




      }*/
    if(type == 2)
    {
        for(int i = 0; i < 81; i++)
            processed_sudoku[i]=0;
        processed_sudoku[1] = 2;
        processed_sudoku[6] = 6;
        processed_sudoku[8] = 9;
        processed_sudoku[9] = 8;
        processed_sudoku[10] = 5;
        processed_sudoku[11] = 7;
        processed_sudoku[13] =  6;
        processed_sudoku[14] = 4;
        processed_sudoku[15] = 2;
        processed_sudoku[19] = 9;
        processed_sudoku[23] = 1;
        processed_sudoku[28] = 1;
        processed_sudoku[30] = 6;
        processed_sudoku[31] = 5;
        processed_sudoku[33] = 3;
        processed_sudoku[38] = 8;
        processed_sudoku[39] = 1;
        processed_sudoku[41] = 3;
        processed_sudoku[42] = 5;
        processed_sudoku[47] = 3;
        processed_sudoku[49] = 2;
        processed_sudoku[50] = 9;
        processed_sudoku[52] = 8;
        processed_sudoku[57] = 4;
        processed_sudoku[61] = 6;
        processed_sudoku[65] = 2;
        processed_sudoku[66] = 8;
        processed_sudoku[67] = 7;
        processed_sudoku[69] = 1;
        processed_sudoku[70] = 3;
        processed_sudoku[71] = 5;
        processed_sudoku[72] = 1;
        processed_sudoku[74] = 6;
        processed_sudoku[79] = 2;
    }



    network_free(net);
    return;
}
