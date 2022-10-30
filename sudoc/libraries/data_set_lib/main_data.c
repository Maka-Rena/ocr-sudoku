#include "include/image.h"

int main()
{
    int number_imgs = 10;
    Image** imgs = csv_to_imgs("DataSet/mnist_test.csv",number_imgs);
    for(int i = 0 ; i < 10; i++)
        img_print(imgs[i]);
}
