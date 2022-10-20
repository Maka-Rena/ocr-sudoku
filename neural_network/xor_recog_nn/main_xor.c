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

    //Now we initialize the weights and biases with random values
    Matrix *hidden_weights = matrix_create(inputLayerNeurons,hiddenLayerNeurons);//OK
    //two for loops to fill the matrix with random numbers 
    //IMPORTANT : NEED TO CHANGE IT TO FUNCTION AND PUT IT IN OPS.C
    for(int i = 0; i < hidden_weights->rows; i++)
        for(int j = 0; j < hidden_weights->cols; j++)
            hidden_weights -> entries[i][j] = rand_number();
    //matrix_print(hidden_weights);
    Matrix *hidden_biases = matrix_create(1,hiddenLayerNeurons);
    for(int i = 0; i < hidden_biases->rows; i++)
        for(int j = 0; j < hidden_biases->cols; j++)
            hidden_biases->entries[i][j] = rand_number();
    //matrix_print(hidden_biases);
    Matrix *output_weights = matrix_create(hiddenLayerNeurons,outputLayerNeurons);//OK
    for(int i = 0; i < output_weights->rows; i++)
        for(int j = 0; j < output_weights->cols; j++)
            output_weights->entries[i][j] = rand_number();
    //matrix_print(output_weights);
    Matrix *output_biases = matrix_create(1,outputLayerNeurons);//OK
    //for the output_biases it is a matrix of n rows and 1 col
    for(int i = 0; i < output_biases->rows; i++)
        output_biases->entries[i][0] = rand_number();
    matrix_print(output_biases);

    //Now we move to the activation of the our layers
    Matrix *hidden_layer_activation = dot(inputs,hidden_weights);
    printf("hidden layer activation\n");
    matrix_print(hidden_layer_activation);
    //matrix_print(hidden_biases);
    hidden_layer_activation = add(hidden_layer_activation, hidden_biases); //PROBLEM : MISSMATCH IN ADDITION
    //matrix_print(hidden_layer_activation);
    //And we apply the sigmoid function on our activated hidden layer
    Matrix *hidden_layer_output = matrix_create(hidden_layer_activation->rows,hidden_layer_activation->cols);
    for(int i = 0; i < hidden_layer_output -> rows;i++)
        for(int j = 0; j < hidden_layer_output -> cols; j++)
            hidden_layer_output -> entries[i][j] = sigmoid(hidden_layer_output -> entries[0][i]);
    //matrix_print(hidden_layer_output);

    Matrix *output_layer_activation = dot(hidden_layer_output, output_weights);
    output_layer_activation = add(output_layer_activation,output_biases);
    Matrix *predicted_output = matrix_create(1,1);
    predicted_output -> entries[0][0] = sigmoid(output_layer_activation -> entries[0][0]);
    matrix_print(predicted_output);

}
