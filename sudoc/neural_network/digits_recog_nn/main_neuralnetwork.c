#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "include/neural_network.h"
#include "include/load_and_save.h"

int main()
{
    srand(time(NULL));

    // TRAINING
      /*int number_imgs = 10000;
      Image** imgs = csv_to_imgs("../../libraries/data_set_lib/DataSet/mnist_test.csv", number_imgs);
      NeuralNetwork* net = network_create(784, 300, 10, 3.0f);
      network_train_batch_imgs(net, imgs, number_imgs);
      network_save(net, "testing_net");
      network_print(net);*/

    // PREDICTING
    /*int number_imgs = 100;
    Image** imgs = csv_to_imgs("../../libraries/data_set_lib/DataSet/mnist_test.csv", number_imgs);
    NeuralNetwork* net = network_load("testing_net");
    //double score = network_predict_imgs(net, imgs, 1000);
    //printf("Score: %1.5f", score);

    matrix_print(imgs[5]->img_data); 
    Matrix* res = network_predict_img(net,imgs[5]);
    printf("predicted number = %i\n", matrix_argmax(res));
    printf("actual number = %i\n", imgs[5]->label);
    imgs_free(imgs, number_imgs);
    network_free(net);*/
    
    // LOADING
    NeuralNetwork* net = network_load("to_load_dir");
    network_print(net);
    network_save(net, "saved_net");
    network_free(net);
    return 0;
}
