#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main_xor.h"
#include "../../libraries/matrix_lib/matrix.h"
#include "../../libraries/matrix_lib/ops.h"

#define inputLayerNeurons 2
#define hiddenLayerNeurons 2
#define outputLayerNeurons 1

/* Sigmoid function
 * x is the activation input
 * returns the activation value */
double sigmoid(double x)
{
    return 1.0 / (1 + exp(-1 * x)); 
}

/* Derivative of sigmoid function for backpropagation purposes
 * x is the input
 * return value of backpropagation*/
double dsigmoid(double x)
{
    return x * (1 - x);
}

/* Function to initialize weights and biases with random values */
double rand_number(){return ((double)rand())/((double)RAND_MAX);}


int main()
{
    Matrix *inputs = matrix_create(4,2);
    //all the values of the matrix are initialized to 0.0f
    //so we only need to fill with ones (following XOR table)

    inputs -> entries[1][1] = 1.0f;
    inputs -> entries[2][0] = 1.0f;
    inputs -> entries[3][0] = 1.0f;
    inputs -> entries[3][1] = 1.0f;
    

    Matrix *expected_outputs = matrix_create(4,1);
    expected_outputs -> entries[1][0] = 1.0f;
    expected_outputs -> entries[2][0] = 1.0f;

    //matrix_print(inputs);
    //matrix_print(expected_outputs);

    //Initializing hidden weights biases with random values
    Matrix *hidden_weights = matrix_create(inputLayerNeurons,hiddenLayerNeurons);//OK
    matrix_randomize(hidden_weights,hiddenLayerNeurons);
    matrix_print(hidden_weights);
    Matrix *hidden_biases = matrix_create(1,hiddenLayerNeurons);
    matrix_randomize(hidden_biases,hiddenLayerNeurons);
    matrix_print(hidden_biases);

    //Initializing output weights and biases with random values
    Matrix *output_weights = matrix_create(hiddenLayerNeurons,outputLayerNeurons);//OK
    matrix_randomize(output_weights,outputLayerNeurons);
    matrix_print(output_weights);
    Matrix *output_biases = matrix_create(1,outputLayerNeurons);
    matrix_randomize(output_biases,outputLayerNeurons);
    matrix_print(output_biases);

    // Activation of hidden Layer
    Matrix *hidden_layer_activation = dot(inputs,hidden_weights); // rows = 2 and cols = 4
    printf("hidden layer activation\n");
    matrix_print(hidden_layer_activation);
    
    //We add the two elements contained in hidden_biases to the corresponding
    //two elements for each column in hidden_layer_activation
    for(int j = 0; j < hidden_layer_activation->rows; j++)
            for(int i = 0; i < hidden_biases -> cols ; i++)
                hidden_layer_activation->entries[j][i] = hidden_layer_activation->entries[j][i] + hidden_biases->entries[0][i];
    printf("new value of hidden_layer_activation\n");
    matrix_print(hidden_layer_activation);

    //We apply the sigmoid function to the hidden_layer_activation elements to
    //get the hidden_layer_output
    //It is important to initiate the matrix before associating it to a new
    //matrix (the return of the function apply for example)
    Matrix *hidden_layer_output = matrix_create(hidden_layer_activation->rows,hidden_layer_activation->cols);
    hidden_layer_output = apply(sigmoid,hidden_layer_activation);
    printf("hidden layer output\n");
    matrix_print(hidden_layer_output);
    
    //We do the same for the output_layer_activation to get the predicted output
    Matrix *output_layer_activation = dot(hidden_layer_output, output_weights);
    printf("output_layer_activation :\n");
    matrix_print(output_layer_activation);

    for(int j = 0; j < output_layer_activation->rows; j++)
        for(int i = 0; i < output_biases->cols; i++)
            output_layer_activation->entries[j][i] = output_layer_activation->entries[j][i] + output_biases->entries[0][i];
    printf("output_layer_activation :\n");
    matrix_print(output_layer_activation);
    Matrix *predicted_output = matrix_create(output_layer_activation->rows,output_layer_activation->cols);
    predicted_output = apply(sigmoid,output_layer_activation);
    printf("predicted_output :\n");
    matrix_print(predicted_output);

    //BackPropagation:

}
