#pragma once

#include "../../libraries/matrix_lib/ops.h"

double sigmoid(double x);
Matrix* sigmoidPrime(Matrix* m);
double leaky_relu(double x);
Matrix* softmax(Matrix* m);
