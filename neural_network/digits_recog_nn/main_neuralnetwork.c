#include <stdlib.h>
#include <stdio.h>
#include "neural_network.h"
#include "load_and_save.h"

int main()
{
    NeuralNetwork* o = network_load("to_load_dir");
    //printf("%i\n",o->input);
    //printf("%i\n",o->hidden);
    //printf("%i\n",o->output);
    //matrix_print(o->hidden_weights);
    //matrix_print(o->output_weights);
    network_save(o,"to_save_dir");
}
