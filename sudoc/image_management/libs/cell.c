#include "../include/cell.h"

float isd(Matrix* matrix)
{
    float total = 0;
    for(int i = 0 ; i < matrix-> rows ; i++)
        for(int j = 0 ; j < matrix -> cols ; j++)
        {
            total += !( matrix->entries[i][j]);
        }
    return total;
}
