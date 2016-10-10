#include <iostream>
#include <algorithm>
#include "buffer.h"
#include "pixel.h"


namespace raytracer {

Buffer::Buffer(const int width_, const int height_) {
    // Init Members
    width = width_;
    height = height_;
    pixel_count = width * height;

    // Init Pixels
    pixels = std::vector<Pixel>(pixel_count);
    std::fill(pixels.begin(), pixels.begin() + pixel_count, Pixel());
}

}
