#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "main_xor.h"
//#include "../../libraries/matrix_lib/matrix.h"
//#include "../../libraries/matrix_lib/ops.h"

#define numInputs 2 //inputLayerNeurons
#define numHiddenNodes 2 //hiddenLayerNeurons
#define numOutputs 1 //outputLayerNeurons
#define numTrainingSets 4

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
double init_weights(){return ((double)rand())/((double)RAND_MAX);}

void shuffle(int *array, size_t n)
{
    if(n > 1)
    {
        for(size_t i = 0; i < n-1; i++)
        {
            size_t j = i + rand()/(RAND_MAX/(n-i)+1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}


int main()
{
    const double lr = 0.1f; //Learning Rate

    double hiddenLayer[numHiddenNodes];
    double outputLayer[numOutputs];

    double hiddenLayerBias[numHiddenNodes];
    double outputLayerBias[numOutputs];

    double hiddenWeights[numInputs][numHiddenNodes];
    double outputWeights[numHiddenNodes][numOutputs];

    double training_inputs[numTrainingSets][numInputs] = {{0.0f,0.0f},
        {1.0f,0.0f},
        {0.0f,1.0f},
        {1.0f,1.0f}};
    double training_outputs[numTrainingSets][numOutputs] = {{0.0f},{1.0f},{1.0f},{0.0f}};

    for(int i = 0; i < numInputs; i++)
        for(int j = 0; j < numHiddenNodes; j++)
            hiddenWeights[i][j] = init_weights();


    for(int i = 0; i < numHiddenNodes; i++)
        for(int j = 0; j < numOutputs; j++)
            outputWeights[i][j] = init_weights();

    for(int i = 0; i < numOutputs; i++)
        outputLayerBias[i] = init_weights();

    int trainingSetOrder[] = {0,1,2,3};
    int numberOfEpochs = 100000;

    //Training
    for(int epoch = 0; epoch < numberOfEpochs; epoch++)
    {
        shuffle(trainingSetOrder, numTrainingSets);

        for(int x = 0; x < numTrainingSets; x++)
        {
            int i = trainingSetOrder[x];

            //Forward propagation
            //
            //Compute hidden Layer activation
            for(int j = 0; j < numHiddenNodes; j++)
            {
                double activation = hiddenLayerBias[j];

                for(int k = 0; k < numInputs; k++)
                {
                    activation += training_inputs[i][j] * hiddenWeights[k][j];
                }

                hiddenLayer[j] = sigmoid(activation);
            }

            //Compute output Layer activation
            for(int j = 0; j < numOutputs; j++)
            {
                double activation = outputLayerBias[j];
                for(int k = 0; k < numHiddenNodes; k++)
                {
                    activation += hiddenLayer[k] * outputWeights[k][j];
                }
                outputLayer[j] = sigmoid(activation);
            }

            printf("Input:%g  %g Output:%g   Expected Output: %g \n", training_inputs[i][0], training_inputs[i][1],outputLayer[0],training_outputs[i][0]);

            //Backprop
            //
            //Compute change in output weights
            //
            double deltaOutput[numOutputs];
            for(int j = 0; j < numOutputs; j++)
            {
                double error = (training_outputs[i][j] - outputLayer[j]);
                deltaOutput[j] = error * dsigmoid(outputLayer[j]);
            }

            // Compute change in hidden weights
            double deltaHidden[numHiddenNodes];
            for(int j = 0; j < numHiddenNodes; j++)
            {
                double error = 0.0f;
                for(int k = 0; k < numOutputs; k++)
                {
                    error += deltaOutput[k] * outputWeights[j][k];
                }
                deltaHidden[j] = error * dsigmoid(hiddenLayer[j]);
            }

            // Apply change in output weights
            for(int j = 0; j < numOutputs; j++)
            {
                outputLayerBias[j] += deltaOutput[j] * lr;
                for(int k = 0; k < numHiddenNodes; k++)
                {
                    outputWeights[k][j] += hiddenLayer[k] * deltaOutput[j] * lr;
                }
            }

            // Apply change in hidden weights
            for(int j = 0; j < numHiddenNodes; j++)
            {
                hiddenLayerBias[j] += deltaHidden[j] * lr;
                for(int k = 0; k < numInputs; k++)
                {
                    hiddenWeights[k][j] += training_inputs[i][k] * deltaHidden[j] * lr;
                }
            }
        }
    }
}
