#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main_xor.h"
#include "../../libraries/matrix_lib/matrix.h"
#include "../../libraries/matrix_lib/ops.h"

#define inputLayerNeurons 2
#define hiddenLayerNeurons 2
#define outputLayerNeurons 1
#define epochs 100000
#define learning_rate 0.4f
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

Matrix* sigmoidPrime(Matrix* m)
{
    Matrix* ones = matrix_create(m -> rows, m-> cols);
    matrix_fill(ones,1);
    Matrix* substracted = substract(ones,m);
    Matrix* multiplied = multiply(m,substracted);
    matrix_free(ones);
    matrix_free(substracted);
    return multiplied;
}

/* Function to initialize weights and biases with random values */
double rand_number(){return ((double)rand())/((double)RAND_MAX);}


int main()
{
    Matrix *inputs = matrix_create(2,4);
    //all the values of the matrix are initialized to 0.0f
    //so we only need to fill with ones (following XOR table)

    inputs -> entries[1][1] = 1.0f;
    inputs -> entries[0][2] = 1.0f;
    inputs -> entries[0][3] = 1.0f;
    inputs -> entries[1][3] = 1.0f;


    Matrix *expected_output = matrix_create(1,4);
    expected_output -> entries[0][1] = 1.0f;
    expected_output -> entries[0][2] = 1.0f;

    //matrix_print(inputs);
    //matrix_print(expected_output);

    //Initializing hidden weights biases with random values
    Matrix *hidden_weights = matrix_create(hiddenLayerNeurons,inputLayerNeurons);//2x2
    matrix_randomize(hidden_weights,11);
    /*Matrix *hidden_bias = matrix_create(1,hiddenLayerNeurons);
      matrix_randomize(hidden_bias,11);
      matrix_print(hidden_bias);*/

    //Initializing output weights with random values
    Matrix *output_weights = matrix_create(outputLayerNeurons, hiddenLayerNeurons);//1x2
    matrix_randomize(output_weights,11);
    printf("output_weights :\n");
    matrix_print(output_weights);
    /*Matrix *output_bias = matrix_create(1,outputLayerNeurons);
      matrix_randomize(output_bias,11);
      printf("output_bias :\n");
      matrix_print(output_bias);*/
    //Matrix *predicted_output = matrix_create(expected_output->rows, expected_output->cols);
    //printf("TEEEEEEEEEEEEEEEEEEEEEEEEEEEEEESSSSSSSSSSSSSST\n");
    //matrix_print(final_output);

    //Starting Training
    //for(int epoch = 0; epoch < 10000 ; epoch++)
    //{
    for(int i = 0; i < epochs; i++)
    {
        //Forward Propagation
        // Activation of hidden Layer
        Matrix *hidden_layer_input = dot(hidden_weights,inputs); // 2x4
        //printf("hidden layer input\n");
        //matrix_print(hidden_layer_input);

        //We apply the sigmoid function to the hidden_layer_activation elements to
        //get the hidden_layer_output
        Matrix *hidden_layer_output = apply(sigmoid,hidden_layer_input); //2x4
        //printf("hidden layer output\n");
        //matrix_print(hidden_layer_output);

        //We do the same for the output_layer_activation to get the predicted output
        Matrix *final_input = dot(output_weights,hidden_layer_output);//1x4
        //printf("final_input :\n");
        //matrix_print(final_input);

        Matrix *final_output = apply(sigmoid, final_input);//1x4
        //printf("final_output :\n");
        //matrix_print(final_output);
        //End of Forward Propagation


        //BackPropagation:
        //We first compute the error
        Matrix* output_errors = substract(expected_output, final_output);//1x4
        //printf("error :\n");
        //matrix_print(output_errors);
        Matrix* hidden_errors = dot(transpose(output_weights),output_errors);//2x4
        //matrix_print(hidden_errors);

        Matrix* sigmoid_primed_mat = sigmoidPrime(final_output);//1x4
        Matrix* multiplied_mat = multiply(output_errors, sigmoid_primed_mat);//1x4
        Matrix* transposed_mat = transpose(hidden_layer_output);//4x2
        Matrix* dot_mat = dot(multiplied_mat, transposed_mat);//1x2
        Matrix* scaled_mat = scale(learning_rate,dot_mat);//1x2

        Matrix* added_mat = add(output_weights, scaled_mat);//1x2
        matrix_free(output_weights);
        output_weights = added_mat;

        // freeing memory to reuse the variables after
        matrix_free(sigmoid_primed_mat);
        matrix_free(multiplied_mat);
        matrix_free(transposed_mat);
        matrix_free(dot_mat);
        matrix_free(scaled_mat);

        sigmoid_primed_mat = sigmoidPrime(hidden_layer_output);//2x4
        multiplied_mat = multiply(hidden_errors, sigmoid_primed_mat);//2x4
        transposed_mat = transpose(inputs);//4x2
        dot_mat = dot(multiplied_mat,transposed_mat);//2x2
        scaled_mat = scale(learning_rate, dot_mat);//2x2
        added_mat = add(hidden_weights, scaled_mat);//2x2
        matrix_free(hidden_weights);
        hidden_weights = added_mat;

        //freeing memory at the end
        matrix_free(sigmoid_primed_mat);
        matrix_free(multiplied_mat);
        matrix_free(transposed_mat);
        matrix_free(dot_mat);
        matrix_free(scaled_mat);

        //freeing matrices
        matrix_free(hidden_layer_input);
        matrix_free(hidden_layer_output);
        matrix_free(final_input);
        //matrix_free(final_output);
        matrix_free(output_errors);
        matrix_free(hidden_errors);

        printf("\n\n--------------------------------------------------------\n\n");
        //matrix_print(hidden_weights);
        //printf("final hidden bias: \n");
        //matrix_print(hidden_bias);
        //printf("final output weights: \n");
        //matrix_print(output_weights);
        //printf("final output bias: \n");
        //matrix_print(output_bias);
        //printf("\nfinal_output :\n");
        matrix_print(final_output);
        matrix_free(final_output);
    }
}
