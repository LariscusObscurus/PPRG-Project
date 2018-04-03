#ifndef COMBINE_H
#define COMBINE_H
#include "Image/Image.h"

template <typename T>
class combine
{
    void combine_in_range(image_matrix<T>& result, image_matrix<T>& operand1, image_matrix<T>& operand2, const size_t begin, const size_t end)
    {
        assert(result.size() == operand1.size());
        assert(operand1.size() == operand2.size());

        Eigen::Map<Eigen::VectorXf> operand1_vector(operand1.data(), operand1.size());
        Eigen::Map<Eigen::VectorXf> operand2_vector(operand2.data(), operand2.size());
        Eigen::Map<Eigen::VectorXf> result_vector(result.data(), result.size());

        for (auto i = begin; i < end; ++i)
        {
            const auto square1 = operand1_vector(i);
            const auto square2 = operand2_vector(i);
            T combined_item = std::sqrt(square1 * square1 + square2 * square2);

            result_vector(i) = combined_item;
        }
    }
public:
    image_matrix<T> & combine_matrices_parallel(image_matrix<T>& result, image_matrix<T>& operand1, image_matrix<T>& operand2)
    {
        auto begin = 0;
        auto end = result.size();

        parallel_for(
            blocked_range<size_t>(begin, end),
            [&](blocked_range<size_t> range)
        {
            combine_in_range(result, operand1, operand2, range.begin(), range.end());
        });

        return result;
    }

    image_matrix<T>& combine_matrices_sequentially(image_matrix<T>& result, image_matrix<T>& operand1, image_matrix<T>& operand2)
    {
        const size_t begin = 0;
        const size_t end = result.size();

        combine_in_range(result, operand1, operand2, begin, end);

        return result;
    }
};

#endif