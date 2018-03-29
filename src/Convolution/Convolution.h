#ifndef CONVOLUTION_H
#define CONVOLUTION_H
#include <Eigen/Dense>
#include <tbb/tbb.h>

using namespace tbb;
using Eigen::Matrix;

template <typename T>
class Convolution
{
    void apply_kernel_in_xy_range(image_matrix<T>& result, image_matrix<T>& operand, image_matrix<T>& kernel,
                                  size_t x_begin, size_t x_end, size_t y_begin, size_t y_end)
    {
        assert(kernel.cols() % 2 == 1);
        assert(kernel.rows() % 2 == 1);
        assert(result.cols() == operand.cols());
        assert(result.rows() == operand.rows());

        size_t kernel_radius_y = (kernel.rows() - 1) / 2;
        size_t kernel_radius_x = (kernel.cols() - 1) / 2;

        assert(x_begin >= (kernel.cols() - 1) / 2);
        assert(x_end < operand.cols() - (kernel_radius_x / 2));
        assert(y_begin >= (kernel.rows() - 1) / 2);
        assert(y_end < operand.rows() - (kernel_radius_y / 2));

        for (size_t y = y_begin; y < y_end; ++y)
        {
            for (size_t x = x_begin; x < x_end; ++x)
            {
                result(y, x) = 0;
                for (size_t y_kernel = 0; y_kernel < kernel.rows(); ++y_kernel)
                {
                    for (size_t x_kernel = 0; x_kernel < kernel.cols(); ++x_kernel)
                    {
                        auto y_operand = y + y_kernel - kernel_radius_y;
                        auto x_operand = x + x_kernel - kernel_radius_x;

                        const float partial_product = operand(y_operand, x_operand) * kernel(y_kernel, x_kernel);

                        result(y, x) += partial_product;
                    }
                }
            }
        }
    }

public:
    image_matrix<T>& apply_kernel_sequentially(image_matrix<T>& result, image_matrix<T>& operand,
                                               image_matrix<T>& kernel)
    {
        size_t kernel_radius_y = (kernel.rows() - 1) / 2;
        size_t kernel_radius_x = (kernel.cols() - 1) / 2;

        size_t x_begin = kernel_radius_x;
        size_t x_end = operand.cols() - kernel_radius_x;
        size_t y_begin = kernel_radius_y;
        size_t y_end = operand.rows() - kernel_radius_y;

        apply_kernel_in_xy_range(result, operand, kernel, x_begin, x_end, y_begin,
                                 y_end);

        return result;
    }

    image_matrix<T>& apply_kernel_parallel(image_matrix<T>& result, image_matrix<T>& operand, image_matrix<T>& kernel)
    {
        size_t kernel_radius_y = (kernel.rows() - 1) / 2;
        size_t kernel_radius_x = (kernel.cols() - 1) / 2;

        size_t x_begin = kernel_radius_x;
        size_t x_end = operand.cols() - kernel_radius_x;
        size_t y_begin = kernel_radius_y;
        size_t y_end = operand.rows() - kernel_radius_y;

        parallel_for(
            blocked_range<size_t>(y_begin, y_end),
            [&](blocked_range<size_t> y_range)
            {
                parallel_for(
                    blocked_range<size_t>(x_begin, x_end),
                    [&](blocked_range<size_t> x_range)
                    {
                        apply_kernel_in_xy_range(result, operand, kernel,
                                                 x_range.begin(), x_range.end(),
                                                 y_range.begin(), y_range.end());
                    });
            });

        return result;
    }
};
#endif
