#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "ops.h"

int main()
{
    Matrix *m1 = matrix_create(2,3);
    Matrix *m2 = matrix_create(3,2);
    m1 -> entries[0][0] = 3;
    m1 -> entries[0][1] = 2;
    m1 -> entries[0][2] = 4;
    m1 -> entries[1][0] = 9;
    m1 -> entries[1][1] = 7;
    m1 -> entries[1][2] = 6;
    m2 -> entries[0][0] = 1;
    m2 -> entries[0][1] = 5;
    m2 -> entries[1][0] = 3;
    m2 -> entries[1][1] = 9;
    m2 -> entries[2][0] = 7;
    m2 -> entries[2][1] = 4;
    matrix_print(m1);
    matrix_print(m2);
    Matrix *m3 = dot(m1,m2);
    matrix_print(m3);
}
