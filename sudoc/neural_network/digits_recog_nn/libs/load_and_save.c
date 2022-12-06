#include "../include/load_and_save.h"

#define MAXCHAR 1000
NeuralNetwork* network_load(char* dir_name)
{
    /*NeuralNetwork* net = malloc(sizeof(NeuralNetwork));
    // Go to the directory where the Neural Network was loaded
    if(chdir(dir_name) == 0)
    {
        printf("Entered in %s\n",dir_name);
    }
    FILE* descriptor = fopen("descriptor", "r");
    // Use of & to associate the value to the address of input
    int input;
    int hidden;
    int output;
    // Condition to avoid the warnings
    if(fscanf(descriptor, "%d", &input)&&fscanf(descriptor, "%d", &hidden)&&fscanf(descriptor, "%d", &output))
    {
        net->input = input;
        net->hidden = hidden;
        net->output = output;
    }
    fclose(descriptor);
    net->hidden_weights = matrix_load("hidden_weights");
    net->output_weights = matrix_load("output_weights");
    //Add later for the bias
    //net->hidden_bias = matrix_load("hidden_bias");
    //net->output_bias = matrix_load("output_bias");
    printf("Network succesfully loaded\n");
    // Go back to the previous directory
    if(chdir("..") == 0)
    {
        printf("Returned to original directory\n");
    }
    return net;*/
    NeuralNetwork* net = malloc(sizeof(NeuralNetwork));
	char entry[MAXCHAR];
	chdir(dir_name);

	FILE* descriptor = fopen("descriptor", "r");
	
    fgets(entry, MAXCHAR, descriptor);
	net->input = atoi(entry);
	fgets(entry, MAXCHAR, descriptor);
	net->hidden = atoi(entry);
	fgets(entry, MAXCHAR, descriptor);
	net->output = atoi(entry);
	fclose(descriptor);
	
    net->hidden_weights = matrix_load("hidden");

	net->output_weights = matrix_load("output");
	printf("Successfully loaded network from '%s'\n", dir_name);
	chdir(".."); // Go back to the original directory
	return net;
}

void network_save(NeuralNetwork* net,char* dir_name)
{
    /*
    // Create a new directory
    mkdir(dir_name, 0777);
    // Go into the new directory
    if(chdir(dir_name) == 0)
    {
        printf("Entered in new directory\n");
    }
    // Create the descriptor file
    FILE* descriptor = fopen("descriptor", "w");
    fprintf(descriptor,"%i\n%i\n%i\n",net->input,net->hidden,net->output);
    fclose(descriptor);
    // Create the files to save the matrices
    matrix_save(net->hidden_weights, "hidden_weights");
    matrix_save(net->output_weights, "output_weights");
    //Add later for the bias
    //matrix_save(net->hidden_bias, "hidden_bias");
    //matrix_save(net->output_bias, "output_bias");
    printf("Network successfully saved\n");
    // Go back to the previous directory
    if(chdir("..") == 0)
    {
        printf("Returned to original directory\n");
    }
    */
   mkdir(dir_name, 0777);
	// Write the descriptor file
	chdir(dir_name);

	FILE* descriptor = fopen("descriptor", "w");
	fprintf(descriptor, "%d\n", net->input);
	fprintf(descriptor, "%d\n", net->hidden);
	fprintf(descriptor, "%d\n", net->output);
	fclose(descriptor);
	matrix_save(net->hidden_weights, "hidden");
	matrix_save(net->output_weights, "output");
	printf("Successfully written to '%s'\n", dir_name);
	chdir("-"); // Go back to the orignal directory
}
