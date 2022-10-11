#pragma once

#include "matrix.h"

double sigmoid(double x);
Matrix* sigmoidPrime(double Matrix* m);
Matrix* softmax(Matrix* m);

