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
#include "./neural_network/digits_recog_nn/play_neuralnetwork.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc != 4)
        errx(1, "Usage: %s <image_path> N type\n", argv[0]);
    
    printf("STARTING PROGRAM\n");
    printf("\n--- CALLING TREATMENT ---\n\n");
    // NEED TO CALL PLAY TREATMENT
    play_treatment(argv[1]);
    printf("\n--- CALLING NEURAL NETWORK ---\n\n");
    // CALL NEURAL NETWORK
    int N = atoi(argv[2]);
    int type = atoi(argv[3]);
    int max = N*N;
    int sudokInit[max];
    play_neuralnetwork(type, N, sudokInit);
    
    //CALL SOLVER
    printf("--- CALLING SOLVER ---\n\n");
    int *sudok = malloc(max * sizeof(int));
    memcpy(sudok, sudokInit, max * sizeof(int));
    
    if (solveSudoku(sudok, 0, 0, N) == 0)
        errx(EXIT_FAILURE, "No solution exists for the sudoku.");
    display_sudoku(sudok, N);
    extract_sudoku(sudok, sudokInit, max);
    return 0;
}