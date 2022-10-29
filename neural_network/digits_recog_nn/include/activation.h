#pragma once
#include <math.h>
#include "../../../libraries/matrix_lib/include/ops.h"

double sigmoid(double x);
Matrix* sigmoidPrime(Matrix* m);
double leaky_relu(double x);
Matrix* softmax(Matrix* m);

