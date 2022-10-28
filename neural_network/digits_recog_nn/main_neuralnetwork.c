#include <stdlib.h>
#include <stdio.h>
#include "neural_network.h"
#include "load_and_save.h"

int main()
{
    NeuralNetwork* net = malloc(sizeof(NeuralNetwork));
    net = network_load("baptisto");
    printf("%i\n",net->input);
    printf("%i\n",net->hidden);
    printf("%i\n",net->output);
    matrix_print(net->hidden_weights);
    matrix_print(net->output_weights);
    if(chdir("-")){}
    network_save(net,"baptistou");
}
