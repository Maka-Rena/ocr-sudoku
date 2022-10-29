#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "neural_network.h"
#include "../../../libraries/data_set_lib/include/image.h"
#include "../../../libraries/matrix_lib/include/matrix.h"

void network_save(NeuralNetwork* net,char* dir_name);

NeuralNetwork* network_load(char* dir_name);
