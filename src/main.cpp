#include <iostream>
#include "Image/Image.h"
#include <tbb/parallel_for.h>


int main(int argc, char*argv[])
{
    tbb::parallel_for(tbb::blocked_range<size_t>(0, 10), [](const tbb::blocked_range<size_t>& i)
    {
        std::cout << "hello world" << std::endl;
    });

	auto test_image = image_io("C:/Users/Leonhardt/Pictures/t.PNG");

    auto image =test_image.get_image();


    test_image.write("C:/Users/Leonhardt/Pictures/t.jpeg", image);

    std::getchar();
    return 0;
}
