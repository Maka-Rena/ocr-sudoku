#include <math.h>
#include "../include/activation.h"

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
Matrix* sigmoidPrime(Matrix* m) 
{
    Matrix* ones = matrix_create(m->rows, m->cols);
    matrix_fill(ones, 1);
    Matrix* substracted = substract(ones, m);
    Matrix* multiplied = multiply(m, substracted);
    matrix_free(ones);
    matrix_free(substracted);
    return multiplied;
}
Matrix* softmax(Matrix* m) 
{
    double total = 0;
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            total += exp(m->entries[i][j]);
        }
    }
    Matrix* mat = matrix_create(m->rows, m->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->entries[i][j] = exp(m->entries[i][j]) / total;
        }
    }
    return mat;
}
