#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/activation.h"
#include "../include/neural_network.h"

NeuralNetwork* network_create(int input, int hidden, int output, double lr)
{
    NeuralNetwork* network = malloc(sizeof(NeuralNetwork));
    network->input = input;
    network->hidden = hidden;
    network->output = output;
    network->learning_rate = lr;
    Matrix* hidden_layer = matrix_create(hidden,input);
    Matrix* output_layer = matrix_create(output,hidden);
    matrix_randomize(hidden_layer,hidden);
    matrix_randomize(output_layer,output);
    //need to create biases
    network->hidden_weights = hidden_layer;
    network->output_weights = output_layer;
    return network;
} 
