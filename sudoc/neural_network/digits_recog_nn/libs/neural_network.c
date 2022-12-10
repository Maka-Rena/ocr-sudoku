#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/activation.h"
#include "../include/neural_network.h"

#define MAXCHAR 1000
#define RED "\033[31;1m"
#define GREEN "\033[32;1m"
#define YELLOW "\033[33;1m"
#define RESET "\033[37;1m"

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

void network_print(NeuralNetwork* net) 
{
    printf("# of Inputs: %d\n", net->input);
    printf("# of Hidden: %d\n", net->hidden);
    printf("# of Output: %d\n", net->output);
    printf("Hidden Weights: \n");
    //matrix_print(net->hidden_weights);
    printf("Output Weights: \n");
    //matrix_print(net->output_weights);
}

void network_free(NeuralNetwork *net) {
    matrix_free(net->hidden_weights);
    matrix_free(net->output_weights);
    free(net);
    net = NULL;
}
// shuffle images during training
void shuffle(Image** imgs, int n) 
{
    for (int i = 0; i < n; i++) 
    {
        int j = rand() % n;
        Image* temp = imgs[i];
        imgs[i] = imgs[j];
        imgs[j] = temp;
    }
}

void network_train(NeuralNetwork* net, Matrix* input, Matrix* output) 
{
    // Feed forward
    Matrix* hidden_inputs = dot(net->hidden_weights, input);
    Matrix* hidden_outputs = apply(sigmoid, hidden_inputs);
    Matrix* final_inputs = dot(net->output_weights, hidden_outputs);
    Matrix* final_outputs = apply(sigmoid, final_inputs);

    // Find errors
    Matrix* output_errors = substract(output, final_outputs);
    Matrix* hidden_errors = dot(transpose(net->output_weights), output_errors);

    // Backpropogate
    //Matrix *output_weights = add(output_weights,scale(net->learning_rate,dot(multiply(output_errors,sigmoidPrime(final_outputs)),transpose(hidden_outputs))));
    Matrix* sigmoid_primed_mat = sigmoidPrime(final_outputs);
    Matrix* multiplied_mat = multiply(output_errors, sigmoid_primed_mat);
    //Matrix* hidden_errors = dot(transpose(net->output_weights), multiplied_mat);
    Matrix* transposed_mat = transpose(hidden_outputs);
    Matrix* dot_mat = dot(multiplied_mat, transposed_mat);
    Matrix* scaled_mat = scale(net->learning_rate, dot_mat);
    Matrix* added_mat = add(net->output_weights, scaled_mat); // was add
    matrix_free(net->output_weights); // Free the old weights before replacing
    net->output_weights = added_mat;

    matrix_free(sigmoid_primed_mat);
    matrix_free(multiplied_mat);
    matrix_free(transposed_mat);
    matrix_free(dot_mat);
    matrix_free(scaled_mat);

    //Matrix *hidden_weights = add(net->hidden_weights, scale(net->learning_rate, dot(multiply(hidden_errors,sigmoidPrime(hidden_outputs)),transpose(input))));
    // Reusing variables after freeing memory
    sigmoid_primed_mat = sigmoidPrime(hidden_outputs);
    multiplied_mat = multiply(hidden_errors, sigmoid_primed_mat);
    transposed_mat = transpose(input);
    dot_mat = dot(multiplied_mat, transposed_mat);
    scaled_mat = scale(net->learning_rate, dot_mat);
    added_mat = add(net->hidden_weights, scaled_mat);// was add
    matrix_free(net->hidden_weights); // Free the old hidden_weights before replacement
    net->hidden_weights = added_mat; 

    matrix_free(sigmoid_primed_mat);
    matrix_free(multiplied_mat);
    matrix_free(transposed_mat);
    matrix_free(dot_mat);
    matrix_free(scaled_mat);

    // Free matrices
    matrix_free(hidden_inputs);
    matrix_free(hidden_outputs);
    matrix_free(final_inputs);
    matrix_free(final_outputs);
    matrix_free(output_errors);
    matrix_free(hidden_errors);
}

void network_train_batch_imgs(NeuralNetwork* net, Image** imgs, int batch_size) 
{
    for(int j = 0 ; j < 10; j++) // 10 epochs
    {
        shuffle(imgs, batch_size); // Shuffle the images
        for (int i = 0; i < batch_size; i++) 
        {
            if (i % 100 == 0) 
            {
                printf(YELLOW "\rTraining Loading -> Img No. %d" RESET, i); // Print progress
                fflush(stdout);
            }
            Image* cur_img = imgs[i];
            Matrix* img_data = matrix_flatten(cur_img->img_data, 0); // 0 = flatten to column vector
            Matrix* output = matrix_create(10, 1);
            output->entries[cur_img->label][0] = 1; // Setting the result
            network_train(net, img_data, output);
            matrix_free(output);
            matrix_free(img_data);
        }
    }
}

Matrix* network_predict_img(NeuralNetwork* net, Image* img) 
{
    Matrix* img_data = matrix_flatten(img->img_data, 0); // 0 = flatten to column vector
    Matrix* res = network_predict(net, img_data);
    matrix_free(img_data);
    return res;
}

double network_predict_imgs(NeuralNetwork* net, Image** imgs, int n) 
{
    int n_correct = 0;
    for (int i = 0; i < n; i++) 
    {
        Matrix* prediction = network_predict_img(net, imgs[i]);
        if (matrix_argmax(prediction) == imgs[i]->label) 
        {
            printf(GREEN "Actual Output : %i -----------> Prediction : %i\n" RESET, imgs[i]->label, matrix_argmax(prediction));
            n_correct++;
        }
        else
        {
            printf(RED "Actual Output : %i -----------> Prediction : %i\n" RESET, imgs[i]->label, matrix_argmax(prediction));
        }
        matrix_free(prediction);
    }
    return 1.0 * n_correct / n;
}

Matrix* network_predict(NeuralNetwork* net, Matrix* input_data) 
{
    //printf("hidden_weights\n");
    //matrix_print(net->hidden_weights);
    //printf("input data");
    //matrix_print(input_data);
    //printf("input_data : row = %i  col = %i\n",input_data->rows,input_data->cols);
    //printf("hidden_weights : row = %i  col = %i\n", net->hidden_weights->rows, net->hidden_weights->cols);
    Matrix* hidden_inputs = dot(net->hidden_weights, input_data);// first arg should not be transposed but was
    Matrix* hidden_outputs = apply(sigmoid, hidden_inputs);
    Matrix* final_inputs = dot(net->output_weights, hidden_outputs); // first arg should not be transposed but was
    //Matrix* final_outputs = apply(sigmoid, final_inputs);
    Matrix* result = apply(sigmoid,final_inputs);
    //Matrix* result = softmax(final_outputs);
    return result;
}
