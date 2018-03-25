#include <iostream>
#include "Image/Image.h"


int main(int argc, char*argv[])
{
	auto test_image = image_io("C:/Users/Leonhardt/Pictures/t.PNG");

    auto image =test_image.get_image();


    test_image.write("C:/Users/Leonhardt/Pictures/t.jpeg", image);

    std::getchar();
    return 0;
}
