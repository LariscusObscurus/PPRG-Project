#include <iostream>
#include "Image/Image.h"
#include <tbb/parallel_for.h>
#include "Convolution/Convolution.h"


int main(int argc, char*argv[])
{
    tbb::parallel_for(tbb::blocked_range<size_t>(0, 10), [](const tbb::blocked_range<size_t>& i)
    {
        std::cout << "hello world" << std::endl;
    });

	auto test_image = image_io("C:/Users/Leonhardt/Pictures/t.PNG");
    image_matrix<float> image = test_image.get_image().cast<float>();

    image_matrix<float> sobel_h(3, 3);
    sobel_h << 1, 2, 1,
        0, 0, 0,
        -1, -2, -1;

    image_matrix<float> sobel_v(3, 3);
    sobel_v << 1, 0, -1,
            2, 0, -2,
            1, 0, -1;

    Convolution<float> convolution;
    auto result = image_matrix<float>(image.rows(), image.cols());
    //convolution.apply_kernel_sequentially(result, image, sobel_h);
    convolution.apply_kernel_parallel(result, image, sobel_h);
    


    test_image.write("C:/Users/Leonhardt/Pictures/t.jpeg", result.cast<unsigned char>());

    std::getchar();
    return 0;
}
