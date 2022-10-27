#include "load_and_save.h"

void network_save(NeuralNetwork* net/*,char* dir_name*/)
{
    //mkdir(dir_name, 0777);
    FILE* descriptor = fopen("descriptor", "w");
    fprintf(descriptor, "%d\n", net->input);
    fprintf(descriptor, "%d\n", net->hidden);
    fprintf(descriptor, "%d\n", net->output);
    fclose(descriptor);
    matrix_save(net->hidden_weights, "hidden_weights");
    matrix_save(net->output_weights, "output_weights");
    //matrix_save(net->hidden_bias, "hidden_bias");
    //matrix_save(net->output_bias, "output_bias");
    //printf("Successfully written to '%s' \n",dir_name);
    printf("Network successfully saved\n");
}

NeuralNetwork* network_load(char* data_file)
{
    NeuralNetwork* net = malloc(sizeof(NeuralNetwork));
    FILE* descriptor = fopen(data, "r");
    int input;
    fscanf(descriptor, "%d", &input); // Use of & to associate the value to the address of input
    int hidden;
    fscanf(descriptor, "%d", &hidden); // Use of & to associate the value to the address of hidden
    int output; 
    fscanf(descriptor, "%d", &output); // Use of & to associate the value to the address of output
    net->input = input;
    net->hidden = hidden;
    net->output = output;
    fclose(descriptor);
    net->hidden_weights = matrix_load("hidden_weights");
    net->output_weights = matrix_load("output_weights");
    net->hidden_bias = matrix_load("hidden_bias");
    net->output_bias = matrix_load("output_bias");
    printf("Network succesfully loaded\n");
    return net;
}
