#ifndef IMAGE_H
#define IMAGE_H
#include <memory>
#include <string>
#include <Eigen/Dense>

namespace stb
{
	#define STB_IMAGE_IMPLEMENTATION 
	#include <stb_image.h>
	#define STB_IMAGE_WRITE_IMPLEMENTATION
	#include <stb_image_write.h>
}

using Eigen::Matrix;

template<typename T>
using image_matrix = Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

class image_io
{
	int width_;
	int height_;
    int original_image_channels_;

    static const int actual_channels = 1;

	std::unique_ptr<unsigned char> image_data_;

public:
	image_io(const std::string& path) 
		: width_(0),
	      height_(0),
		  original_image_channels_(0),
	      image_data_(stb::stbi_load(path.c_str(), &width_, &height_, &original_image_channels_, actual_channels))
	{
	}

    ~image_io() = default;

    image_io(image_io& other) = delete;
    image_io& operator=(image_io& other) = delete;

	image_io(image_io&& other) noexcept
		: width_(other.width_),
	      height_(other.height_),
	      original_image_channels_(other.original_image_channels_),
	      image_data_(std::move(other.image_data_))
	{
	}

	image_io& operator=(image_io&& other) noexcept
	{
        if (this == &other)
            return *this;

        image_data_ = std::move(other.image_data_);
        width_ = other.width_;
        height_ = other.height_;
        original_image_channels_ = other.original_image_channels_;
        return *this;
	}

	void write(const std::string& path, image_matrix<unsigned char> matrix) const
	{
		stb::stbi_write_jpg(path.c_str(), width_, height_, 1, matrix.data(), 90);
	}

    image_matrix<unsigned char> get_image() const
    {
        static_assert(actual_channels == 1, "Update this method if you want to use more than one channel.");
        auto map = Eigen::Map<image_matrix<unsigned char>, 0>(image_data_.get(), height_, width_);
        return map.matrix();
	}
};

#endif