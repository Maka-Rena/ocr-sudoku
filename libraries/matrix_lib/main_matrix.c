#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "ops.h"

int main()
{
    Matrix* m = matrix_load("toto");
    matrix_print(m);
}
