#include <iostream>
#include "Image/Image.h"
#include <tbb/parallel_for.h>
#include "Convolution/Convolution.h"
#include "Combine/Combine.h"


int main(int argc, char*argv[])
{
	auto test_image = image_io("C:/Users/Leonhardt/Pictures/Lenna.png");
    image_matrix<float> image = test_image.get_image().cast<float>();

    image_matrix<float> sobel_h(3, 3);
    sobel_h << 1, 2, 1,
        0, 0, 0,
        -1, -2, -1;

    image_matrix<float> sobel_v(3, 3);
    sobel_v << 1, 0, -1,
            2, 0, -2,
            1, 0, -1;

    convolution<float> convolution;
    auto result_h = image_matrix<float>(image.rows(), image.cols());
    auto result_v = image_matrix<float>(image.rows(), image.cols());
    auto combined = image_matrix<float>(image.rows(), image.cols());

    //convolution.apply_kernel_sequentially(result, image, sobel_h);
    convolution.apply_kernel_parallel(result_h, image, sobel_h);
    convolution.apply_kernel_parallel(result_v, image, sobel_v);

    combine<float> combine;
    combine.combine_matrices_parallel(combined, result_h, result_v);
    


    test_image.write("C:/Users/Leonhardt/Pictures/out.png", combined.cast<unsigned char>());

    std::getchar();
    return 0;
}
