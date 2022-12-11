#include "./solver/include/parser.h"
#include "./solver/include/display.h"
#include "./solver/include/solver.h"
#include "./rotate_image/include/rotate.h"
#include "./neural_network/digits_recog_nn/include/activation.h"
#include "./neural_network/digits_recog_nn/include/load_and_save.h"
#include "./neural_network/digits_recog_nn/include/neural_network.h"
#include "./libraries/matrix_lib/include/matrix.h"
#include "./libraries/matrix_lib/include/ops.h"
#include "./libraries/data_set_lib/include/image.h"
#include "./image_management/include/play_treatment.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 2)
        errx(1, "Usage: %s <image_path>\n", argv[0]);
    
    printf("STARTING PROGRAM\n");
    // NEED TO CALL PLAY TREATMENT
    play_treatment(argv[1]);
    // NEED TO CALL SOLVER
    return 0;
}