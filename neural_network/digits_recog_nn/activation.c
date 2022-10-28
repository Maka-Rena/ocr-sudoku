#include <math.h>
#include "activation.h"

double sigmoid(double x)
{
    return 1.0 / (1 + exp(-1 * x));
}
double leaky_relu(double x)
{
    if(x > 0)
        return x;
    else
        return 0.1*x;
}

