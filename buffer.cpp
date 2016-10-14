#include <iostream>
#include <algorithm>
#include "buffer.h"
#include "color.h"


namespace raytracer {

Buffer::Buffer(const int width_, const int height_) {
    // Init Members
    width = width_;
    height = height_;
    pixel_count = width * height;

    // Init Pixels
    pixels = std::vector<Color>(pixel_count);
    std::fill(pixels.begin(), pixels.begin() + pixel_count, Color());
}

} // end namespace
