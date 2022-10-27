#pragma once
#include "../../libraries/matrix_lib/matrix.h"
#include "utilities/image.h"

typedef struct{
    int input;
    int hidden;
    int output;
    double learning_rate;
    Matrix* hidden_weights;
    Matrix* output_weights;
    //Matrix* hidden_biases;
    //Matrix* output_biases;
}NeuralNetwork;

NeuralNetwork* network_create(int input, int hiddden, int output, double lr);
void network_train(NeuralNetwork* net, Matrix* input_data, Matrix* output_data);
void network_train_batch_imgs(NeuralNetwork* net, Image** imgs, int batch_size);
Matrix* network_predict_img(NeuralNetwork* net, Image* img);
double network_predict_imgs(NeuralNetwork* net, Image** imgs, int n);
Matrix* network_predict(NeuralNetwork* net, Matrix* input_data);
void network_save(NeuralNetwork* net, char* file_string);
NeuralNetwork* network_load(char* file_string);
void network_print(NeuralNetwork* net);
void network_free(NeuralNetwork* net);
