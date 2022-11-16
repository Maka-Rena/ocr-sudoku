#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "include/neural_network.h"
#include "include/load_and_save.h"

int main(int argc, char** argv)
{
    srand(time(NULL));
    if(strcmp(argv[1], "train") == 0)
        // TRAINING
    {
        int number_imgs = 60000;
        Image** imgs = csv_to_imgs("../../libraries/data_set_lib/DataSet/mnist_train.csv", number_imgs);
        NeuralNetwork* net = network_create(784, 30, 10, 0.06f);
        network_train_batch_imgs(net, imgs, number_imgs);
        network_save(net, "testing_net");
        network_print(net);

        network_free(net);
    }
    // PREDICTING
    if(strcmp((argv[1]), "predict") == 0 )
    {
        int number_imgs = 10000;
        Image** imgs = csv_to_imgs("../../libraries/data_set_lib/DataSet/mnist_test.csv", number_imgs);
        NeuralNetwork* net = network_load("testing_net");
        double score = network_predict_imgs(net, imgs, 10000);
        printf("Score: %1.5f\n", score);
        
        network_free(net);
    }

    /*matrix_print(imgs[5]->img_data); 
      Matrix* res = network_predict_img(net,imgs[5]);
      printf("predicted number = %i\n", matrix_argmax(res));
      printf("actual number = %i\n", imgs[5]->label);
      imgs_free(imgs, number_imgs);*/
    return 0;
}
