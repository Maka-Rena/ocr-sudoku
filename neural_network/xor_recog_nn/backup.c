#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main_xor.h"
#include "../../libraries/matrix_lib/matrix.h"
#include "../../libraries/matrix_lib/ops.h"

#define inputLayerNeurons 2
#define hiddenLayerNeurons 2
#define outputLayerNeurons 1
#define epochs 1000
#define learning_rate 0.1f
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
    return x * (1.0 - x);
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


    Matrix *expected_output = matrix_create(4,1);
    expected_output -> entries[1][0] = 1.0f;
    expected_output -> entries[2][0] = 1.0f;

    //matrix_print(inputs);
    //matrix_print(expected_output);

    //Initializing hidden weights biases with random values
    Matrix *hidden_weights = matrix_create(inputLayerNeurons,hiddenLayerNeurons);//OK
    matrix_randomize(hidden_weights,11);
    matrix_print(hidden_weights);
    Matrix *hidden_bias = matrix_create(1,hiddenLayerNeurons);
    matrix_randomize(hidden_bias,11);
    matrix_print(hidden_bias);

    //Initializing output weights and biases with random values
    Matrix *output_weights = matrix_create(hiddenLayerNeurons,outputLayerNeurons);//OK
    matrix_randomize(output_weights,11);
    printf("output_weights :\n");
    matrix_print(output_weights);
    Matrix *output_bias = matrix_create(1,outputLayerNeurons);
    matrix_randomize(output_bias,11);
    printf("output_bias :\n");
    matrix_print(output_bias);
    //Matrix *predicted_output = matrix_create(expected_output->rows, expected_output->cols);
    //printf("TEEEEEEEEEEEEEEEEEEEEEEEEEEEEEESSSSSSSSSSSSSST\n");
    //matrix_print(final_output);

    //Starting Training
    //for(int epoch = 0; epoch < 10000 ; epoch++)
    //{
    //Forward Propagation
    // Activation of hidden Layer
    Matrix *hidden_layer_activation = dot(inputs,hidden_weights); // rows = 2 and cols = 4
    //printf("hidden layer activation\n");
    //matrix_print(hidden_layer_activation);

    //We add the two elements contained in hidden_biases to the corresponding
    //two elements for each column in hidden_layer_activation
    for(int j = 0; j < hidden_layer_activation->rows; j++)
        for(int i = 0; i < hidden_bias -> cols ; i++)
            hidden_layer_activation->entries[j][i] = hidden_layer_activation->entries[j][i] + hidden_bias->entries[0][i];
    //printf("new value of hidden_layer_activation\n");
    //matrix_print(hidden_layer_activation);

    //We apply the sigmoid function to the hidden_layer_activation elements to
    //get the hidden_layer_output
    //It is important to initiate the matrix before associating it to a new
    //matrix (the return of the function apply for example)
    Matrix *hidden_layer_output = matrix_create(hidden_layer_activation->rows,hidden_layer_activation->cols);
    hidden_layer_output = apply(sigmoid,hidden_layer_activation);
    //printf("hidden layer output\n");
    //matrix_print(hidden_layer_output);

    //We do the same for the output_layer_activation to get the predicted output
    Matrix *output_layer_activation = dot(hidden_layer_output, output_weights);
    //printf("output_layer_activation :\n");
    //matrix_print(output_layer_activation);

    for(int j = 0; j < output_layer_activation->rows; j++)
        for(int i = 0; i < output_bias->cols; i++)
            output_layer_activation->entries[j][i] = output_layer_activation->entries[j][i] + output_bias->entries[0][i];
    //printf("output_layer_activation after addition:\n");
    //matrix_print(output_layer_activation);
    Matrix *predicted_output = matrix_create(output_layer_activation->rows,output_layer_activation->cols);
    predicted_output = apply(sigmoid,output_layer_activation);
    //printf("predicted_output :\n");
    //matrix_print(predicted_output);
    //End of Forward Propagation

    //BackPropagation:
    //We first compute the error
    Matrix* error = substract(expected_outputs, predicted_output);
    printf("error :\n");
    matrix_print(error);

    /*Matrix *predicted_output_dsigmoided = matrix_create(predicted_output->rows, predicted_output->cols);
    predicted_output_dsigmoided = apply(dsigmoid, predicted_output);
    printf("dsigmoid predicted_output :\n");
    matrix_print(predicted_output_dsigmoided);
    Matrix *d_predicted_output = multiply(error, predicted_output_dsigmoided);
    printf("d_predicted_output :\n");
    matrix_print(d_predicted_output);

    Matrix *error_hidden_layer = matrix_create(hiddenLayerNeurons,1);
    printf("error_hidden_layer :\n");
    matrix_print(error_hidden_layer);
    for(int j = 0; j < error_hidden_layer->rows; j++)
    {
        double err = 0.0f;
        for(int k = 0; k < outputLayerNeurons; k++)
        {
            err+= d_predicted_output->entries[k][0]*output_weights->entries[j][k];
        }
        error_hidden_layer->entries[j][0] = err*dsigmoid(hidden_layer_output -> entries[j][0]);
    }
    matrix_print(error_hidden_layer);//NEED TO VERIFY THE RESULT
    //Now we apply the changes in the output weights
    for(int j = 0; j < outputLayerNeurons; j++)
    {
        output_biases->entries[j][0] += d_predicted_output->entries[j][0] * learning_rate;
        for(int k = 0 ; k < hiddenLayerNeurons ; k++)
        {
            output_weights->entries[k][j] += hidden_layer_output->entries[k][0] * d_predicted_output->entries[j][0] * learning_rate;
        }
    }
    printf("output_biases backprop :\n");
    matrix_print(output_biases);
    printf("output_weights backprop :\n");
    matrix_print(output_weights);
    //Now we apply the changes in the hidden weights
    printf("debuging loop :\n------> hidden_biases :\n");
    matrix_print(hidden_biases);
    printf("------> error_hidden_layer :\n");
    matrix_print(error_hidden_layer);

    for(int j = 0; j < hiddenLayerNeurons; j++)
    {
        hidden_biases-> entries[0][j] += error_hidden_layer->entries[j][0] * learning_rate;
        for(int k = 0 ; k < inputLayerNeurons ; k++)
        {
            hidden_weights->entries[k][j] += inputs->entries[k][0] * error_hidden_layer->entries[j][0] * learning_rate;
        }
    }
    printf("hidden_biases backprop :\n");
    matrix_print(hidden_biases);
    printf("hidden_weights backprop :\n");
    matrix_print(hidden_weights);
    for(int i = 0; i < final_output->rows; i++)
        for(int j = 0; j < final_output-> cols;j++)
            final_output->entries[i][j] = predicted_output->entries[i][j];
    //}*/
    printf("final hidden weights: \n");
    matrix_print(hidden_weights);
    printf("final hidden bias: \n");
    matrix_print(hidden_bias);
    printf("final output weights: \n");
    matrix_print(output_weights);
    printf("final output bias: \n");
    matrix_print(output_bias);
    printf("\npredicted_output :\n");
    matrix_print(predicted_output);
}
