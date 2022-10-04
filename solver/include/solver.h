#pragma once
#define N 9
#include <stdio.h>
#include <stdlib.h>

int isAvailable(int puzzle[], int row, int col, int num);
int solveSudoku(int *sudok, int row, int col);
