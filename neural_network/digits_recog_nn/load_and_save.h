#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "neural_network.h"
#include "utilities/image.h"
//include "matrix.h"

void network_save(NeuralNetwork* net/*,char* dir_name*/);

NeuralNetwork* network_load(char* data_file);
