#include <iostream>
#include <algorithm>
#include "buffer.h"


namespace raytracer {

Buffer::Buffer(const int width_, const int height_)
{
    // Init Members
    width = width_;
    height = height_;
    pixel_count = width * height;

    // Init Pixels
    pixels = std::vector<Color>(pixel_count);
    clear();
}

void Buffer::clear()
{
    // Empty
    std::fill(pixels.begin(), pixels.begin() + pixel_count, Color());
}

} // end namespace
