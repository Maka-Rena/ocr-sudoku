#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}
double dSegmoid(double x)
{
    return x * (1 - x);
}
double init_weights()
{ 
    return ((double)rand()) / ((double)RAND_MAX); 
}

void shuffle(int *array, size_t n)
{
    if(n > 1)
    {
        size_t i;
        for(i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }

    }
}

#define numInputs 2
#define numHiddenNodes 2
#define numOutputs 1
#define numTrainingSets 4

int main(void)
{
    const double lr = 0.1f;

    double hiddenLayer[numHiddenNodes];
    double outputLayer[numOutputs];

    double hiddenLayerBias[numHiddenNodes];
    double outputLayerBias[numOutputs];

    double hiddenWeights[numInputs][numHiddenNodes];
    double outputWeights[numHiddenNodes][numOutputs];

    double training_inputs[numTrainingSets][numInputs] = {{0.0f, 0.0f},
        {1.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f}};

    double training_outputs[numTrainingSets][numOutputs] = {{0.0f},
        {1.0f},
        {1.0f},
        {0.0f}};
    for(int i = 0 ; i < numInputs; i++)
    {
        for(int j = 0; i < numHiddenNodes; j++)
        {
            hiddenWeights[i][j] = init_weights();
        }
    }
    for(int i = 0 ; i < numInputs; i++)
    {
        for(int j = 0; i < numHiddenNodes; j++)
        {
            hiddenWeights[i][j] = init_weights();
        }
    }
}
