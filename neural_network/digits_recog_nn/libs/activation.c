#include <math.h>
#include "activation.h"
#include "ops.h"

double sigmoid(double x)
{
    return 1.0 / (1 + exp(-1 * x));
}
