#pragma once
#include <stdio.h>
#include <stdlib.h>

int isAvailable(int puzzle[], int row, int col, int num, int n);
int solveSudoku(int *sudok, int row, int col, int n);
