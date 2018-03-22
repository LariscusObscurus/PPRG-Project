#include <iostream>
#include <Eigen/Dense>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

using Eigen::MatrixXd;

int main(int argc, char*argv[])
{
	using namespace boost::gil;
	rgb8_image_t img;
	png_read_image("C:/Users/Leonhardt/Pictures/t.PNG", img);
	png_write_view("C:/Users/Leonhardt/Pictures/t1.PNG", img);
	MatrixXd m(2, 2);
	m(0, 0) = 3;
	m(1, 0) = 2.5;
	m(0, 1) = -1;
	m(1, 1) = m(1, 0) + m(0, 1);
	std::cout << m << std::endl;
	std::getchar();
	return 0;
}
