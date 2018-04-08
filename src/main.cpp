#include <iostream>
#include "Image/Image.h"
#include <tbb/parallel_for.h>
#include "Convolution/Convolution.h"
#include "Combine/Combine.h"
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>

std::chrono::duration<double, std::milli> run(image_matrix<float> image,
                                              image_io &image_io,
                                              image_matrix<float> sobel_h,
                                              image_matrix<float> sobel_v,
                                              image_matrix<float>& (convolution<float>::*apply_kernel)(
                                                                                 image_matrix<float>&, image_matrix<float>&, image_matrix<float>&),
                                              image_matrix<float>& (combine<float>::*combine_matrices)(
                                                                                 image_matrix<float>&, image_matrix<float>&, image_matrix<float>&))
{
    convolution<float> convolution;
    combine<float> combine;
    auto result_h = image_matrix<float>(image.rows(), image.cols());
    auto result_v = image_matrix<float>(image.rows(), image.cols());
    auto combined = image_matrix<float>(image.rows(), image.cols());

    const auto start_time = std::chrono::steady_clock::now();
    (convolution.*apply_kernel)(result_h, image, sobel_h);
    (convolution.*apply_kernel)(result_v, image, sobel_v);

    (combine.*combine_matrices)(combined, result_h, result_v);

    const auto end_time = std::chrono::steady_clock::now();
    const auto diff = end_time - start_time;

//    image_io.write("./out.png", combined.cast<unsigned char>());

    return diff;
}

void print_stats(std::vector<double> times)
{
    auto minmax = std::minmax_element(times.begin(), times.end());
    auto avg = std::accumulate(times.begin(), times.end(), 0.0) / times.size();

    std::cout
    << "Min: " << times.at(minmax.first - times.begin()) << ", "
    << "Max: " << times.at(minmax.second - times.begin()) << ", "
    << "Average: " << avg
    << std::endl;
}

int main(int argc, char*argv[])
{
    auto test_image = image_io("./t.png");
    image_matrix<float> image = test_image.get_image().cast<float>();

    image_matrix<float> sobel_h(3, 3);
    sobel_h << 1, 2, 1,
        0, 0, 0,
        -1, -2, -1;

    image_matrix<float> sobel_v(3, 3);
    sobel_v << 1, 0, -1,
            2, 0, -2,
            1, 0, -1;

    std::cout << "Running sequentially" << std::endl;
    std::vector<double> times_seq;
    for (int i = 0; i < 10; i++)
    {
        auto diff = run(image, test_image, sobel_h, sobel_v,
                        &convolution<float>::apply_kernel_sequentially,
                        &combine<float>::combine_matrices_sequentially);
        double count = std::chrono::duration<double, std::milli>(diff).count();
        times_seq.push_back(count);
        std::cout << count << " ms" << std::endl;
    }
    print_stats(times_seq);
    std::cout << std::endl;

    std::cout << "Running parallel" << std::endl;
    std::vector<double> times_par;
    for (int i = 0; i < 10; i++)
    {
        auto diff = run(image, test_image, sobel_h, sobel_v,
                   &convolution<float>::apply_kernel_parallel,
                   &combine<float>::combine_matrices_parallel);
        double count = std::chrono::duration<double, std::milli>(diff).count();
        times_par.push_back(count);
        std::cout << count << " ms" << std::endl;
    }
    print_stats(times_par);

    std::getchar();
    return 0;
}
